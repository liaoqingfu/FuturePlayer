#ifndef MODULECOMMON_HPP
#define MODULECOMMON_HPP
#include <module.hpp>

class ModuleCommon
{
public:
    virtual bool set();
protected:
    ModuleCommon();
    ~ModuleCommon();

    void SetModule(Module &m);

    inline Settings &sets()
    {
        return *module;
    }

    inline Module &getModule()
    {
        return *module;
    }

    template<typename T>
    inline void setInstance()
    {
        module->setInstance<T>();
    }
private:
    Module *module = nullptr;
};
#endif // MODULECOMMON_HPP
