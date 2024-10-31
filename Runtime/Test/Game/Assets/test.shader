Inputs
    object
        Type: PushConstant
        Stage: Vertex
        Structure
            transform: Matrix4
    camera
        Type: UniformBuffer
        Stage: Vertex
        Set: 0
        Binding: 0
        Frequent: true
        Structure
            camera: Matrix4
    texture
        Type: CombinedImageSampler
        Stage: Fragment
        Set: 0
        Binding: 1
        Frequent: false
        Structure
            texSampler: Asset
PrimitiveTopology: TriangleList
PolygonMode: Fill
CullMode: Back
FrontFace: Clockwise
LineWidth: 1
Bindings
    - 
        - Float3
        - Float3
        - Float2
Stages
    Vertex: 2544c66cdde6c4b9
    Fragment: 6e13ef98800386f6