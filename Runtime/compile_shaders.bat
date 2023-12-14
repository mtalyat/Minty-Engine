@Rem Compile to spv
C:/Libraries/VulkanSDK/1.3.261.1/Bin/glslc.exe ui.vert -o uivert.spv
C:/Libraries/VulkanSDK/1.3.261.1/Bin/glslc.exe ui.frag -o uifrag.spv
@Rem Convert to base64
certutil -encodehex -f "uivert.spv" "uivertbase64.txt" 0x40000001 1>nul
certutil -encodehex -f "uifrag.spv" "uifragbase64.txt" 0x40000001 1>nul
pause