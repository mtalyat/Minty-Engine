using System;
using MintyEngine;

class TestScript : Script
{
    void OnCreate()
    {
        Console.WriteLine("TestClass.OnCreate()");
    }

    void OnUpdate()
    {
        Console.WriteLine("TestClass.OnUpdate()");
    }

    void OnDestroy()
    {
        Console.WriteLine("TestClass.OnDestroy()");
    }
}