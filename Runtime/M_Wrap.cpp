#include "pch.h"
#include "M_Wrap.h"

#include "M_VirtualFile.h"
#include "M_PhysicalFile.h"
#include "M_File.h"
#include "M_Console.h"
#include "M_Asset.h"

#include <filesystem>

using namespace minty;

minty::Wrap::Wrap()
    : _path()
    , _header()
    , _entries()
    , _indexed()
{}

minty::Wrap::Wrap(Path const& path)
    : _path(path)
    , _header()
    , _entries()
    , _indexed()
{
    load();
}

minty::Wrap::Wrap(Path const& path, String const& name, uint32_t const entryCount, Path const& base, uint32_t const contentVersion)
    : _path(path)
    , _header()
    , _entries()
    , _indexed()
{
    // set header data
    _header.type = Type::File;
    _header.wrapVersion = WRAP_VERSION;
    _header.contentVersion = contentVersion;
    set_name(name);
    set_base_path(base);
    _header.entryCount = entryCount;

    _entries.resize(entryCount);
    for (uint32_t i = 0; i < entryCount; i++)
    {
        _empties.emplace(i);
    }

    // open file, open with truncate to override any existing file
    PhysicalFile file(path, File::Flags::Write | File::Flags::Binary | File::Flags::Truncate);

    // write header
    write_header(file);

    // write entries
    for (uint32_t i = 0; i < entryCount; i++)
    {
        write_entry(file, i);
    }
    
    file.close();
}

void minty::Wrap::load()
{
    // check if file exists and is valid
    if (!std::filesystem::exists(_path))
    {
        // file does not exist
        Console::error(std::format("Cannot load_animation \"{}\" Wrap file: file does not exist.", _path.string()));
        return;
    }
    else if (!std::filesystem::is_regular_file(_path))
    {
        // not a file
        Console::error(std::format("Cannot load_animation \"{}\" Wrap file: not a regular file.", _path.string()));
        return;
    }
    else if (_path.extension() != WRAP_EXTENSION)
    {
        // not a .wrap file
        Console::error(std::format("Cannot load_animation \"{}\" Wrap file: missing .wrap file extension.", _path.string()));
        return;
    }

    // open the file
    PhysicalFile file(_path, File::Flags::Read | File::Flags::Binary);

    // read header data
    file.read(&_header, sizeof(Header));

    // ensure it is a valid wrap file by checking the magic data
    if (memcmp(_header.id, WRAP_MAGIC, WRAP_MAGIC_SIZE))
    {
        // does not have the correct "WRAP" id magic
        Console::error(std::format("Cannot emplace \"{}\" into Wrap file: invalid data.", _path.string()));
        _header = Header();
        return;
    }

    // read entries
    Path basePath(_header.basePath);
    _entries.resize(_header.entryCount);
    _indexed.reserve(_header.entryCount);
    for (uint32_t i = 0; i < _header.entryCount; i++)
    {
        // read entry
        Entry& entry = _entries.at(i);
        file.read(&entry, sizeof(Entry));
        // add path and index
        _indexed.emplace(basePath / entry.path, i);
        // add to empties if empty
        if (entry.empty())
        {
            _empties.emplace(i);
        }
    }

    file.close();
}

void minty::Wrap::write_header(PhysicalFile& wrapFile) const
{
    wrapFile.seek_write(0);
    wrapFile.write(&_header, sizeof(Header));
}

void minty::Wrap::write_entry(PhysicalFile& wrapFile, size_t const index) const
{
    wrapFile.seek_write(sizeof(Header) + sizeof(Entry) * index);
    wrapFile.write(&_entries.at(index), sizeof(Entry));
}

uint32_t minty::Wrap::emplace_entry(Entry& newEntry)
{
    int64_t bestIndex = -1;

    // search through empty entries to find best fit
    for (uint32_t const index : _empties)
    {
        Entry& entry = _entries.at(index);

        if (entry.reservedSize && entry.reservedSize < newEntry.reservedSize)
        {
            // not big enough
            continue;
        }

        // big enough

        // replace best index if no index yet, or this is a closer reserved size
        if(bestIndex == -1)
        {
            // no best index yet, so place the index unconditionally
            bestIndex = index;
            continue;
        }

        Entry const& bestEntry = _entries.at(bestIndex);
        if ((entry.reservedSize && !bestEntry.reservedSize) || (entry.reservedSize && bestEntry.reservedSize && entry.reservedSize < bestEntry.reservedSize))
        {
            // place if best has no reserved size and this does, OR
            // if both have a size and this one is smaller
            bestIndex = index;
        }
    }

    // if index, replace that entry and return that index
    if (bestIndex >= 0)
    {
        // TODO: if not an exact size match, then split the entry
        uint32_t index = static_cast<uint32_t>(bestIndex);

        // remove index from empties
        _empties.erase(index);

        // replace old indexed path
        _indexed.erase(Path(_header.basePath) / _entries[index].path);

        // move some data to new entry (pos, size, etc.)
        Entry const& oldEntry = _entries.at(index);

        // if there already was a size and offset reserved, use those, otherwise accept the incoming size and offset
        if (oldEntry.reservedSize)
        {
            newEntry.offset = oldEntry.offset;
            newEntry.reservedSize = oldEntry.reservedSize;
        }

        // officially replace it
        _entries[index] = newEntry;
        _indexed[Path(_header.basePath) / newEntry.path] = index;

        // return that index
        return static_cast<uint32_t>(index);
    }

    // cannot fit
    Console::error("Cannot emplace entry to Wrap file. Entry count surpassed.");

    return -1;
}

char const* minty::Wrap::get_base_path() const
{
    return _header.basePath;
}

void minty::Wrap::set_base_path(Path const& path)
{
    // set the base path
    memcpy(_header.basePath, path.string().c_str(), WRAP_HEADER_PATH_SIZE);
    _header.basePath[WRAP_HEADER_PATH_SIZE - 1] = '\0';

    // replace indexed
    _indexed.clear();
    Path basePath(_header.basePath);
    for (size_t i = 0; i < _header.entryCount; i++)
    {
        Entry const& entry = _entries.at(i);
        _indexed.emplace(basePath / entry.path, i);
    }
}

Path const& minty::Wrap::get_path() const
{
    return _path;
}

char const* minty::Wrap::get_name() const
{
    return _header.name;
}

void minty::Wrap::set_name(String const& name)
{
    // set name
    memcpy(_header.name, name.c_str(), WRAP_HEADER_NAME_SIZE);
    _header.name[WRAP_HEADER_NAME_SIZE - 1] = '\0';
}

uint16_t minty::Wrap::get_wrap_version() const
{
    return _header.wrapVersion;
}

uint32_t minty::Wrap::get_content_version() const
{
    return _header.contentVersion;
}

void minty::Wrap::emplace(Path const& physicalPath, Path const& virtualPath, CompressionLevel const compressionLevel, uint32_t const reservedSize)
{
    // check if file exists and is valid
    if (!std::filesystem::exists(physicalPath))
    {
        // file does not exist
        Console::error(std::format("Cannot emplace \"{}\" into Wrap file: file does not exist.", physicalPath.string()));
        return;
    }
    else if (!std::filesystem::is_regular_file(physicalPath))
    {
        // not a file
        Console::error(std::format("Cannot emplace \"{}\" into Wrap file: not a regular file.", physicalPath.string()));
        return;
    }

    // open wrap file
    PhysicalFile wrapFile(_path, File::Flags::ReadWrite | File::Flags::Binary);

    // open file
    PhysicalFile file(physicalPath, File::Flags::Read | File::Flags::Binary);

    // read the data from the file
    File::Size fileSize = file.size();
    Byte* fileData = new Byte[fileSize];
    file.read(fileData, fileSize);

    // done with that file
    file.close();

    // create an entry for the new file
    Entry entry;
    memcpy(entry.path, virtualPath.string().c_str(), WRAP_ENTRY_PATH_SIZE);
    entry.path[WRAP_ENTRY_PATH_SIZE - 1] = '\0';
    entry.compressionLevel = static_cast<Byte>(compressionLevel);
    entry.uncompressedSize = static_cast<uint32_t>(fileSize);

    // compress the data, if needed
    if (static_cast<bool>(compressionLevel))
    {
        // calculate sizes
        unsigned long sourceSize = static_cast<unsigned long>(fileSize);
        unsigned long destSize = compress_bound(sourceSize);

        // create dest buffer
        Byte* compressedData = new Byte[destSize];

        // compress it
        if (compress(compressedData, destSize, fileData, sourceSize, compressionLevel))
        {
            Console::error(std::format("Cannot emplace \"{}\" into Wrap file: failed to compress file with compression level {}.", physicalPath.string(), static_cast<int>(compressionLevel)));
            return;
        }

        // replace source
        delete[] fileData;
        fileData = compressedData;
        fileSize = static_cast<File::Size>(destSize);

        Console::log(std::format("Compressed {} from {} bytes to {} bytes.", physicalPath.string(), std::to_string(sourceSize), std::to_string(destSize)));
    }

    // set size and offset
    entry.offset = static_cast<uint32_t>(wrapFile.size());
    entry.compressedSize = static_cast<uint32_t>(fileSize);
    if (reservedSize)
    {
        entry.reservedSize = reservedSize;
    }
    else
    {
        entry.reservedSize = entry.compressedSize;
    }
    
    // add entry
    size_t index = emplace_entry(entry);

    // write to file
    write_entry(wrapFile, index);
    wrapFile.seek_write(entry.offset, File::Direction::Begin);
    wrapFile.write(fileData, fileSize);

    // cleanup
    delete[] fileData;
    wrapFile.close();
}

bool minty::Wrap::exists(Path const& path) const
{
    return _indexed.contains(path);
}

bool minty::Wrap::contains(Path const& path) const
{
    return exists(path);
}

bool minty::Wrap::open(Path const& path, VirtualFile& file) const
{
    if (!exists(path))
    {
        return false;
    }

    // get entry
    Entry const& entry = get_entry(path);

    // open file in the given virtual file
    file.open(_path, File::Flags::Read | File::Flags::Binary, entry.offset, entry.uncompressedSize);

    return true;
}

std::vector<char> minty::Wrap::read(Path const& path) const
{
    VirtualFile file;

    if (!open(path, file))
    {
        // could not open
        return std::vector<char>();
    }

    // read all data from file
    Entry const& entry = get_entry(path);
    File::Size fileSize = static_cast<File::Size>(entry.compressedSize);
    Byte* fileData = new Byte[fileSize];
    file.read(fileData, fileSize);

    // uncompress it
    unsigned long sourceSize = static_cast<unsigned long>(entry.compressedSize);
    unsigned long size = static_cast<unsigned long>(entry.uncompressedSize);
    Byte* data = new Byte[size];
    if (uncompress(data, size, fileData, sourceSize))
    {
        Console::error(std::format("Failed to uncompress file \"{}\" in Wrap file.", path.string()));
        return std::vector<char>();
    }

    // done with file data
    delete[] fileData;

    // add to vector
    std::vector<char> result(size);
    memcpy(result.data(), data, size);

    // done with data
    delete[] data;

    return result;
}

Wrap::Entry const& minty::Wrap::get_entry(size_t const index) const
{
    return _entries.at(index);
}

Wrap::Entry const& minty::Wrap::get_entry(Path const& path) const
{
    return _entries.at(_indexed.at(path));
}

Wrap::Type minty::Wrap::get_type() const
{
    return _header.type;
}

void minty::Wrap::set_type(Type const type)
{
    _header.type = type;
}

minty::Wrap::Header::Header()
{}

minty::Wrap::Header::Header(Header const& other)
    : id("")
    , type(other.type)
    , wrapVersion(other.wrapVersion)
    , contentVersion(other.contentVersion)
    , basePath("")
    , name("")
    , entryCount(other.entryCount)
{
    memcpy(id, other.id, WRAP_MAGIC_SIZE);
    memcpy(basePath, other.basePath, WRAP_HEADER_PATH_SIZE);
    basePath[WRAP_HEADER_PATH_SIZE - 1] = '\0';
    memcpy(name, other.name, WRAP_HEADER_NAME_SIZE);
    name[WRAP_HEADER_NAME_SIZE - 1] = '\0';
}

Wrap::Header& minty::Wrap::Header::operator=(Header const& other)
{
    if (&other != this)
    {
        memcpy(id, other.id, WRAP_MAGIC_SIZE);
        type = other.type;
        wrapVersion = other.wrapVersion;
        contentVersion = other.contentVersion;
        memcpy(basePath, other.basePath, WRAP_HEADER_PATH_SIZE);
        basePath[WRAP_HEADER_PATH_SIZE - 1] = '\0';
        memcpy(name, other.name, WRAP_HEADER_NAME_SIZE);
        name[WRAP_HEADER_NAME_SIZE - 1] = '\0';
        entryCount = other.entryCount;
    }

    return *this;
}

minty::Wrap::Entry::Entry()
{}

minty::Wrap::Entry::Entry(Entry const& other)
    : path("")
    , compressionLevel(other.compressionLevel)
    , reservedSize(other.reservedSize)
    , compressedSize(other.compressedSize)
    , uncompressedSize(other.uncompressedSize)
    , offset(other.offset)
{
    memcpy(path, other.path, WRAP_ENTRY_PATH_SIZE);
    path[WRAP_ENTRY_PATH_SIZE - 1] = '\0';
}

Wrap::Entry& minty::Wrap::Entry::operator=(Entry const& other)
{
    if (&other != this)
    {
        memcpy(path, other.path, WRAP_ENTRY_PATH_SIZE);
        path[WRAP_ENTRY_PATH_SIZE - 1] = '\0';
        compressionLevel = other.compressionLevel;
        reservedSize = other.reservedSize;
        compressedSize = other.compressedSize;
        uncompressedSize = other.uncompressedSize;
        offset = other.offset;
    }

    return *this;
}

bool minty::Wrap::Entry::empty() const
{
    return uncompressedSize == 0;
}
