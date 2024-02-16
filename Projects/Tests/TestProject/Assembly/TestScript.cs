using System;
using MintyEngine;

class TestScript : Script
{
    void OnCreate()
    {
        Console.WriteLine("TestClass.OnCreate()");
    }

    void OnLoad()
    {
        Console.WriteLine("TestClass.OnLoad()");
    }

    void OnUpdate()
    {
        Console.WriteLine("TestClass.OnUpdate()");
    }

    void OnUnload()
    {
        Console.WriteLine("TestClass.OnUnload()");
    }

    void OnDestroy()
    {
        Console.WriteLine("TestClass.OnDestroy()");
    }
}