using MintyEngine;

class TestScript : Script
{
    public float time;

    private Transform transform;

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

        transform = Entity.GetComponent<Transform>();
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
        
        if (time <= 0.0f)
        {
            Entity.Enabled = false;
        }

        Vector3 pos = transform.LocalPosition;
        pos.Z += Time.ElapsedTime;
        transform.LocalPosition = pos;

        Debug.Log(transform.LocalPosition);
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