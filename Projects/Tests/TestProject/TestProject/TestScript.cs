using MintyEngine;
using System;

class TestScript : Script
{
    public int count = 0;

    void OnCreate()
    {
        Debug.Log("TestClass.OnCreate()");
        Debug.Log($"Entity is {Entity}");
        Entity.Name = "test";
        Debug.Log($"Entity is now {Entity}");
    }

    void OnLoad()
    {
        Debug.Log("TestClass.OnLoad()");
        Debug.Log(Entity.GetComponent<Transform>() == null);
        Entity.AddComponent<Transform>();
        Debug.Log(Entity.GetComponent<Transform>() == null);
        Entity.RemoveComponent<Transform>();
        Debug.Log(Entity.GetComponent<Transform>() == null);
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