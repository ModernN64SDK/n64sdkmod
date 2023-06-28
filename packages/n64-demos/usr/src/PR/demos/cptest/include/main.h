#ifndef __mainHeader__
#define __mainHeader__

typedef struct 
{
    int a;
    int b;
} MyStruct;

class MyClass 
{
public:    
    int i;
    int j;
    
    MyClass(void);
    virtual ~MyClass();
    virtual void NotImp();
};

class Foo
{
protected:
    int tmp;
    MyClass mc;
    
public:
    Foo();
    virtual ~Foo();
    
    virtual void Proc1();
    void Proc2();
};

class Bar : public Foo
{
public:
    Bar();
    virtual ~Bar();
    virtual void Proc1();
    
};

#endif
