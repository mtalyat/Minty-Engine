using MintyEngine;
using System;

class Test
{
    public Entity Entity = new Entity();
}

class TestScript : Script
{
    public int count = 0;

    void OnCreate()
    {
        Debug.Log("TestClass.OnCreate()");
        Debug.Log($"Entity is {(Entity == null ? "null" : "not null")}");
        Debug.Log($"Entity is {Entity}");
    }

    void OnLoad()
    {
        Debug.Log("TestClass.OnLoad()");
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