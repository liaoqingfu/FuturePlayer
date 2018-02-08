#ifndef DEMUXERFACTORY_HPP
#define DEMUXERFACTORY_HPP

#include "demuxer.h"
#include "ffdemux.h"

#include <string>
#include <map>
#include <list>
#include <stdio.h>

//#include "static_utils.h"

class _Static {
    public:
        template<int N, class T>
        static T & _static() {
            //initial once
            static T _instance;
            return _instance;
        }
};

class DemuxerCreatorBase
{
public:
    DemuxerCreatorBase(){}
    virtual Demuxer* Create() = 0;
};

template<class T>
class DemuxerCreator :public DemuxerCreatorBase
{
public:
    DemuxerCreator(const std::string& strkey = "") :m_strKey(strkey){}

    Demuxer* Create()
    {
        //return new T;
        return nullptr;
    }
private:
    std::string m_strKey;
};

class DemuxerFactory
{
    DemuxerFactory(){}

public:
    static DemuxerFactory& Instance()
    {
        static DemuxerFactory s_Instance;

        return s_Instance;
    }

    void AddDemuxerCreator(const std::string& strClassName, DemuxerCreatorBase* pDemuxerCreator)
    {
        if (m_mapDemuxerCreator.find(strClassName) == m_mapDemuxerCreator.end())
        {
            m_mapDemuxerCreator.insert(make_pair(strClassName, pDemuxerCreator));
        }
    }

    Demuxer* Create(const std::string& strKey)
    {
        Demuxer* pZBSObj = NULL;
        std::map<std::string, DemuxerCreatorBase*>::iterator it;

        it = m_mapDemuxerCreator.find(strKey);
        if (it != m_mapDemuxerCreator.end())
        {
            pZBSObj = it->second->Create();
        }

        return pZBSObj;
    }

    int GetMapSize()
    {
        return m_mapDemuxerCreator.size();
    }

private:
    std::map<std::string, DemuxerCreatorBase*> m_mapDemuxerCreator;
};

#define REGISTER_DEMUXER(ClassName)\
    DemuxerFactory::Instance().AddDemuxerCreator(#ClassName,new DemuxerCreator<ClassName>());

extern "C" Demuxer* DemuxerCreate(const std::string& strKey);



#endif // DEMUXERFACTORY_HPP
