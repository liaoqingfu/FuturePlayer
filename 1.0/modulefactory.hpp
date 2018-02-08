#ifndef MODULEFACTORY_HPP
#define MODULEFACTORY_HPP

/**
 * @brief The ModuleFactory class
 * @note 模块工厂，所有的模块都注册到模块工厂中去
 */
/*
class ModuleCreatorBase
{
public:
    ModuleCreatorBase(){}
    virtual Module* Create() = 0;
};

template<class T>
class ModuleCreatorCreator :public ModuleCreatorBase
{
public:
    ModuleCreator(const std::string& strkey = "") :m_strKey(strkey){}

    T* Create()
    {
        return new T;
    }
private:
    std::string m_strKey;
};




#define REGISTER_MODULE_FACTORY(type, ClassName)\
    ModuleFactory::Instance().AddZBSCreator(#ClassName, new ZBSCreator<ClassName>());
*/

class ModuleFactory
{
public:
    enum Type {
        NUL, READER, DEMUXER, DECODER, FILTER, WRITER
    };
    ModuleFactory();
    static ModuleFactory& Instance()
    {
        static ModuleFactory s_Instance;

        return s_Instance;
    }
};
#endif // MODULEFACTORY_HPP
