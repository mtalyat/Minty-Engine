using MintyEngine;

class TestScript : Script
{
    public int count = 0;

    void OnCreate()
    {
        Debug.Log("TestClass.OnCreate()");
    }

    void OnLoad()
    {
        Debug.Warn("TestClass.OnLoad()" + count);
    }

    void OnUpdate()
    {
        Debug.Error("TestClass.OnUpdate()");
        count++;
    }

    void OnUnload()
    {
        Debug.Assert(count >= 5000, "TestClass.OnUnload() under 5k");
        Debug.Assert(count < 5000, "TestClass.OnUnload() over 5k");
    }

    void OnDestroy()
    {
        Debug.Log("TestClass.OnDestroy()", Debug.Color.BrightMagenta);
    }
}