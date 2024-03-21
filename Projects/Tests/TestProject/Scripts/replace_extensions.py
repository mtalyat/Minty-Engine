import os
import shutil

def replace_extension(root_dir, old_ext, new_ext):
    """
    Replace files' extension from old_ext to new_ext in root_dir and its subdirectories.

    :param root_dir: Directory path to start the search from.
    :param old_ext: Old file extension (without dot).
    :param new_ext: New file extension to replace with (without dot).
    """
    old_ext = '.' + old_ext if not old_ext.startswith('.') else old_ext
    new_ext = '.' + new_ext if not new_ext.startswith('.') else new_ext

    for root, dirs, files in os.walk(root_dir):
        for file in files:
            if file.endswith(old_ext):
                # Construct the full file path
                old_file_path = os.path.join(root, file)
                # Construct the new file name with the new extension
                new_file_name = file[:-len(old_ext)] + new_ext
                new_file_path = os.path.join(root, new_file_name)
                # Rename the file
                shutil.move(old_file_path, new_file_path)
                print(f'Renamed "{old_file_path}" to "{new_file_path}"')

# Example usage:
if __name__ == '__main__':
    directory_path = 'Assets'  # Change this to your directory
    old_extension = 'A'  # Change 'A' to your old extension
    new_extension = 'B'  # Change 'B' to your new extension
    replace_extension(directory_path, old_extension, new_extension)
