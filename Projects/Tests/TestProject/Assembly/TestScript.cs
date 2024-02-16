using System;
using MintyEngine;

class TestScript : Script
{
    public int count = 0;

    void OnCreate()
    {
        Console.WriteLine("TestClass.OnCreate()");
    }

    void OnLoad()
    {
        Console.WriteLine("TestClass.OnLoad()" + count);
    }

    void OnUpdate()
    {
        Console.WriteLine("TestClass.OnUpdate()");
        count++;
    }

    void OnUnload()
    {
        Console.WriteLine("TestClass.OnUnload()" + count);
    }

    void OnDestroy()
    {
        Console.WriteLine("TestClass.OnDestroy()");
    }
}