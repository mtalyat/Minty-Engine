:: Deletes any compiled shaders from a given directory.
@echo off

for /r %1 %%f in (*.spv) do (
    echo Deleting "%%f".
    del "%%f"
)