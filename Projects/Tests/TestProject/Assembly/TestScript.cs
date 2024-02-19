using MintyEngine;

class TestScript : Script
{
    public int count = 0;

    void OnCreate()
    {
        System.Console.WriteLine("C#: TestClass.OnCreate()");
        Debug.Log("C++: TestClass.OnCreate()");
    }

    void OnLoad()
    {
        Debug.Log("TestClass.OnLoad()" + count);
    }

    void OnUpdate()
    {
        Debug.Log("TestClass.OnUpdate()");
        count++;
    }

    void OnUnload()
    {
        Debug.Log("TestClass.OnUnload()");
    }

    void OnDestroy()
    {
        Debug.Log("TestClass.OnDestroy()", Debug.Color.BrightMagenta);
    }
}