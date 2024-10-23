set compiler=C:/Libraries/VulkanSDK/1.3.261.1/Bin/glslc.exe

%compiler% ../Data/default/ShaderModule/GLSL/defaultShader.vert -o ../Data/default/ShaderModule/Compiled/defaultVert.spv
%compiler% ../Data/default/ShaderModule/GLSL/defaultShader.frag -o ../Data/default/ShaderModule/Compiled/defaultFrag.spv
%compiler% ../Data/default/ShaderModule/GLSL/defaultSprite.vert -o ../Data/default/ShaderModule/Compiled/defaultSpriteVert.spv
%compiler% ../Data/default/ShaderModule/GLSL/defaultSprite.frag -o ../Data/default/ShaderModule/Compiled/defaultSpriteFrag.spv
%compiler% ../Data/default/ShaderModule/GLSL/defaultUi.vert -o ../Data/default/ShaderModule/Compiled/defaultUiVert.spv
%compiler% ../Data/default/ShaderModule/GLSL/defaultUi.frag -o ../Data/default/ShaderModule/Compiled/defaultUiFrag.spv
pause