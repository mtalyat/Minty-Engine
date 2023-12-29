#include "pch.h"
#include "M_Wrap.h"

#include "M_File.h"
#include "M_Console.h"
#include "M_Asset.h"

#include <filesystem>
#include <zlib.h>

using namespace minty;

minty::Wrap::Wrap()
    : _header()
    , _entries()
{}

minty::Wrap::Wrap(String const& name, Path const& path, uint32_t const contentVersion)
    : _header()
    , _entries()
{
    _header.type = Type::File;
    _header.wrapVersion = WRAP_VERSION;
    _header.contentVersion = contentVersion;
    strncpy_s(_header.name, name.c_str(), WRAP_HEADER_NAME_SIZE);
    strncpy_s(_header.basePath, path.string().c_str(), WRAP_HEADER_PATH_SIZE);
    _header.entryCount = 0;
}

minty::Wrap::Wrap(Path const& path)
    : _header()
    , _entries()
{
    load(path);
}

uint32_t minty::Wrap::get_version() const
{
    return _header.contentVersion;
}

char const* minty::Wrap::get_base_path() const
{
    return _header.basePath;
}

char const* minty::Wrap::get_name() const
{
    return _header.name;
}

uint16_t minty::Wrap::get_wrap_version() const
{
    return _header.wrapVersion;
}

uint32_t minty::Wrap::get_content_version() const
{
    return _header.contentVersion;
}

bool minty::Wrap::contains(Path const& path) const
{
    return _entries.contains(_header.basePath / path);
}

Wrap::Entry& minty::Wrap::at(Path const& path)
{
    return _entries.at(_header.basePath / path);
}

Wrap::Entry const& minty::Wrap::at(Path const& path) const
{
    return _entries.at(_header.basePath / path);
}

Wrap::Type minty::Wrap::get_type() const
{
    return _header.type;
}

void minty::Wrap::set_type(Type const type)
{
    _header.type = type;
}

void minty::Wrap::emplace(Path const& path, bool const recursive, Compression const compression)
{
    Path fixedPath;

    if (path.string().starts_with("."))
    {
        if (path.string().size() > 1)
        {
            // ignore hidden files and folders
            return;
        }
        else
        {
            // use base path
            fixedPath = "";
        }
    }
    else
    {
        // get relative path from base path
        fixedPath = std::filesystem::relative(path, _header.basePath);
    }

    fixedPath = _header.basePath / fixedPath;

    // if directory, load all files within the directory
    // if file, load
    if (std::filesystem::is_directory(fixedPath))
    {
        for (auto const& iter : std::filesystem::directory_iterator(fixedPath))
        {
            Path const& subpath = iter.path();

            // if another directory and recursive, then load it as well
            // if a file, load it
            if (std::filesystem::is_directory(subpath) && recursive)
            {
                emplace(subpath, true, compression);
            }
            else if (std::filesystem::is_regular_file(subpath))
            {
                emplace(subpath, false, compression);
            }
        }
    }
    else if (std::filesystem::is_regular_file(fixedPath))
    {
        // add as a file entry
        Entry entry;
        strncpy_s(entry.path, fixedPath.string().c_str(), WRAP_ENTRY_PATH_SIZE);
        entry.compressed = static_cast<Byte>(compression);
        entry.offset = 0;
        entry.size = 0;
        _entries.emplace(fixedPath, entry);
        console::log("added: " + fixedPath.string());
    }
}

void minty::Wrap::save(Path const& path) const
{
    // write header
    // write entries
    // write all entry's file datas, compress if needed
    
    // open file at path, or create a new one if needed
    PhysicalFile file(path, File::Flags::Write);

    // write header
    file.write(_header);

    // temp write fake entries
    Entry emptyEntry;
    for (size_t i = 0; i < _entries.size(); i++)
    {
        file.write(emptyEntry);
    }

    // now write each file, and calculate the size and offset, then re-write each entry on the file
    size_t headerSize = sizeof(Header);
    size_t entrySize = sizeof(Entry);

    for (auto const& pair : _entries)
    {
        // load file from disk
        PhysicalFile assetFile(pair.first, File::Flags::Read | File::Flags::Binary);

        // determine size
        File::Size assetSize = assetFile.size();


    }
}

void minty::Wrap::load(Path const& path)
{
    if (!std::filesystem::exists(path))
    {
        // invalid path
        console::error(std::format("Cannot load wrap file at path \"{}\": file does not exist.", path.string()));
        return;
    }

    // load wrap file
    PhysicalFile file(path, File::Flags::Read);

    // read the header
    file.read(&_header, sizeof(Header));

    // verify it is a wrap header
    if (std::memcmp(WRAP_MAGIC, _header.id, 4))
    {
        // not "WRAP"
        console::error(std::format("Could not load Wrap file from path \"{}\". Missing magic data.", path.string()));
        return;
    }

    // read all of the entries
    Entry entry;
    for (uint32_t i = 0; i < _header.entryCount; i++)
    {
        file.read(&entry, sizeof(Entry));

        _entries.emplace(entry.path, entry);
    }
}

minty::Wrapper::Wrapper()
    : _wraps()
{}

minty::Wrapper::~Wrapper()
{
}

void minty::Wrapper::emplace(Wrap const& wrap)
{
    // load it into data
    // if the name does not exist OR (the name does exist AND this content version is higher), add
    auto const& found = _wraps.find(wrap.get_name());

    if (found == _wraps.end() || wrap.get_content_version() > found->second.get_content_version())
    {
        // new OR replace
        _wraps[wrap.get_name()] = wrap;
    }

    // determine what to do based on type
    switch (wrap.get_type())
    {
    case Wrap::Type::File:
        break;
    default:
        console::todo(std::format("Wrap file type \"{}\".", static_cast<uint16_t>(wrap.get_type())));
        break;
    }
}

void minty::Wrapper::emplace(Path const& path, bool const recursive)
{
    // if given a path to a directory, load all .wrap files within it
    // otherwise just load the file itself, if its a .wrap file

    if (std::filesystem::is_directory(path)) // directory
    {
        for (auto const& entry : std::filesystem::directory_iterator(path))
        {
            // if a directory and recursive is true
            // or if a file
            if ((entry.is_directory() && recursive) || (entry.is_regular_file()))
            {
                // load the other directory, or the next file
                emplace(entry.path().string(), recursive);
            }
        }
    }
    else if (std::filesystem::is_regular_file(path) && path.has_extension() && path.extension() == ".wrap") // file
    {
        // create the wrap
        Wrap wrap(path.stem().string(), Asset::absolute(path));

        // load it
        wrap.load(path);

        // add it to the wrapper
        emplace(wrap);
    }
}

Wrap const& minty::Wrapper::get(String const& name) const
{
    return _wraps.at(name);
}

ID minty::Wrapper::open(Path const& path)
{
    // find file
    for (auto const& pair : _wraps)
    {
        // if wrap file contains the target path...
        if (pair.second.contains(path))
        {
            // create a virtual file of that entry
            Wrap::Entry const& entry = pair.second.at(path);

            File* file = new VirtualFile(pair.second.get_base_path(), File::Flags::Read | File::Flags::Binary, entry.offset, entry.size);

            ID id = _files.emplace(path.string(), file);

            return id;
        }
    }

    // no file found at path in Wrapper
    return ERROR_ID;
}

File* minty::Wrapper::at(ID const id) const
{
    return _files.at(id);
}

File* minty::Wrapper::at(Path const& path) const
{
    return _files.at(path.string());
}

void minty::Wrapper::close(ID const id)
{
    if (_files.contains(id))
    {
        delete _files.at(id);
        _files.erase(id);
    }
}

void minty::Wrapper::close(Path const& path)
{
    std::string strPath = path.string();

    if (_files.contains(strPath))
    {
        delete _files.at(strPath);
        _files.erase(strPath);
    }
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
    strncpy_s(id, other.id, WRAP_HEADER_ID_SIZE);
    strncpy_s(basePath, other.basePath, WRAP_HEADER_PATH_SIZE);
    strncpy_s(name, other.name, WRAP_HEADER_NAME_SIZE);
}

Wrap::Header& minty::Wrap::Header::operator=(Header const& other)
{
    if (&other != this)
    {
        strncpy_s(id, other.id, WRAP_HEADER_ID_SIZE);
        type = other.type;
        wrapVersion = other.wrapVersion;
        contentVersion = other.contentVersion;
        strncpy_s(basePath, other.basePath, WRAP_HEADER_PATH_SIZE);
        strncpy_s(name, other.name, WRAP_HEADER_NAME_SIZE);
        entryCount = other.entryCount;
    }

    return *this;
}

minty::Wrap::Entry::Entry()
{}

minty::Wrap::Entry::Entry(Entry const& other)
    : path("")
    , compressed(other.compressed)
    , size(other.size)
    , offset(other.offset)
{
    strncpy_s(path, other.path, WRAP_ENTRY_PATH_SIZE);
}

Wrap::Entry& minty::Wrap::Entry::operator=(Entry const& other)
{
    if (&other != this)
    {
        strncpy_s(path, other.path, WRAP_ENTRY_PATH_SIZE);
        compressed = other.compressed;
        size = other.size;
        offset = other.offset;
    }

    return *this;
}
