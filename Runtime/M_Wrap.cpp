#include "pch.h"
#include "M_Wrap.h"

#include "M_File.h"

#include <filesystem>
#include <zlib.h>

using namespace minty;

minty::Wrap::Wrap(Path const& path)
    : _header()
    , _entries()
{
    if (std::filesystem::exists(path))
    {
        // load wrap file
        PhysicalFile file(path, File::Flags::Read);

        // read the header
        file.read(&_header, sizeof(Header));

        // read all of the entries
        Entry entry;
        for (uint32_t i = 0; i < _header.entryCount; i++)
        {
            file.read(&entry, sizeof(Entry));

            _entries.emplace(entry.path, entry);
        }
    }
    else
    {
        // else, use empty wrap file, creating a new one
        String pathstr = path.string();
        _header.type = Type::File;
        strncpy_s(_header.basePath, pathstr.c_str(), min(pathstr.length(), 100));
        strncpy_s(_header.name, Path(path).stem().string().c_str(), 50);
    }
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

Wrap::Entry const* minty::Wrap::get_entry(Path const& path) const
{
    // get the entry if it exists
    String relativePath = std::filesystem::relative(path, _header.basePath).string();

    auto const& found = _entries.find(relativePath);

    if (found == _entries.end())
    {
        return nullptr;
    }
    else
    {
        return &found->second;
    }
}

bool minty::Wrap::contains(Path const& path)
{
    return _entries.contains(path);
}

Wrap::Entry& minty::Wrap::at(Path const& path)
{
    return _entries.at(path);
}

Wrap::Entry const& minty::Wrap::at(Path const& path) const
{
    return _entries.at(path);
}

Wrap::Type minty::Wrap::get_type() const
{
    return _header.type;
}

void minty::Wrap::set_type(Type const type)
{
    _header.type = type;
}

minty::Wrapper::Wrapper(Path const& path)
    : _wraps()
{}

void minty::Wrapper::load(Path const& path, bool const recursive)
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
                load(entry.path().string(), recursive);
            }
        }
    }
    else if (std::filesystem::is_regular_file(path) && path.has_extension() && path.extension() == ".wrap") // file
    {
        // create the wrap
        Wrap wrap(path);

        // load it into data
        _wraps.emplace(wrap.get_name(), wrap);
    }
}
