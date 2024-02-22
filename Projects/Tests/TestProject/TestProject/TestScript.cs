using MintyEngine;
using System;

class TestScript : Script
{
    public float time;

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
    }

    void OnEnable()
    {
        Debug.Log("TestClass.OnEnable()");
    }

    void OnUpdate()
    {
        Debug.Log("TestClass.OnUpdate()");
        Debug.Log($"Time: {time:0.00}s");
        time -= Time.ElapsedTime;
        if(time <= 0.0f)
        {
            //Destroy(this);
            Destroy(Entity);
        }
    }

    void OnDisable()
    {
        Debug.Log("TestClass.OnDisable()");
    }

    void OnUnload()
    {
        Debug.Log("TestClass.OnUnload()");
    }

    void OnDestroy()
    {
        Debug.Log("TestClass.OnDestroy()");
    }
}