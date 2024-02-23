using MintyEngine;

class TestScript : Script
{
    public float time;

    private Transform transform;
    private InputMap inputMap;

    void OnCreate()
    {
        Debug.Log("TestClass.OnCreate()");
        Debug.Log($"Entity is {Entity}");

        inputMap = new InputMap();
        inputMap.AddKeyDown(Key.E, OnKeyDown);
        Input.SetInputMap(inputMap);
    }

    private void OnKeyDown(object sender, KeyPressEventArgs e)
    {
        Debug.Log(e.Key);
        Debug.Log(e.Action);
        Debug.Log(e.Modifiers);
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
        //Debug.Log("TestClass.OnUpdate()");
        //Debug.Log($"Time: {time:0.00}s");
        //time -= Time.ElapsedTime;
        
        //if (time <= 0.0f)
        //{
        //    Destroy(this);
        //}

        //Vector3 pos = transform.LocalPosition;
        //pos.Z += Time.ElapsedTime;
        //transform.LocalPosition = pos;

        //transform.LocalScale /= 1.0f + Time.ElapsedTime * 0.1f;

        //Debug.Log(transform.LocalPosition);
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