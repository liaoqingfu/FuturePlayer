#ifndef MODULE_HPP
#define MODULE_HPP
#include <QStringList>
#include <QWidget>
#include <QMutex>
#include <QIcon>
#include <Settings.hpp>

class ModuleCommon;

/**
 * @brief The Module class
 * 模块的定义：模块不是audio、video decoder，而是类似alsa、ffmpeg、opengl，但他们是怎么串联起来的呢。
 * 以Ffmpeg为例，他是一个module，里面包含了FFDemux、FFDecSW等，FFDemux和FFDecSW又继承了什么呢？最基层的为ModuleCommon
 * createInstance 获得真正的执行类
 */
class Module : public Settings
{
    friend class ModuleCommon;
public:
    enum TYPE {NONE, DEMUXER, DECODER, READER, WRITER, PLAYLIST, QMPLAY2EXTENSION, SUBSDEC, AUDIOFILTER, VIDEOFILTER};
    enum FILTERTYPE {DEINTERLACE = 0x400000, DOUBLER = 0x800000, USERFLAG = 0x80000000};
    enum HWTYPE {VIDEOHWFILTER = 0x4000000};

    inline Module(const QString &mName) :
        Settings(mName),
        mName(mName)
    {}
    virtual ~Module() = default;

    inline QString name() const
    {
        return mName;
    }

    class Info
    {
    public:
        Info() = default;
        inline Info(const QString &name, const quint32 type, const QIcon &icon = QIcon(), const QString &description = QString()) :
            name(name), description(description), type(type), icon(icon)
        {}
        inline Info(const QString &name, const quint32 type, const QString &description) :
            name(name), description(description), type(type)
        {}
        inline Info(const QString &name, const quint32 type, const QStringList &extensions, const QIcon &icon = QIcon(), const QString &description = QString()) :
            name(name), description(description), type(type), icon(icon), extensions(extensions)
        {}

        QString name, description;
        quint32 type = NONE;
        QIcon icon;
        QStringList extensions;
    };
    virtual QList<Info> getModulesInfo(const bool showDisabled = false) const = 0;
    virtual void *createInstance(const QString &) = 0;

    virtual QList<QAction *> getAddActions();

    class SettingsWidget : public QWidget
    {
    public:
        virtual void saveSettings() = 0;
        inline void flushSettings()
        {
            sets().flush();
        }
    protected:
        inline SettingsWidget(Module &module) :
            module(module)
        {
            setAttribute(Qt::WA_DeleteOnClose);
        }

        inline Settings &sets()
        {
            return module;
        }

        template<typename T>
        inline void SetInstance()
        {
            module.setInstance<T>();
        }
    private:
        Module &module;
    };
    virtual SettingsWidget *getSettingsWidget();

    virtual void videoDeintSave();

    inline QIcon icon() const
    {
        return m_icon;
    }

    void setInstances(bool &);

    template<typename T>
    void setInstance();

protected:
    QIcon m_icon;

private:
    QMutex mutex;
    QString mName;
    QList<ModuleCommon *> instances;
};

template<typename T>
void Module::setInstance()
{
    QMutexLocker locker(&mutex);
    for (ModuleCommon *mc : instances)
    {
        T *t = dynamic_cast<T *>(mc);
        if (t)
            t->set();
    }
}

/**/

#define QMPLAY2_MODULES_API_VERSION 6

#define QMPLAY2_EXPORT_MODULE(ModuleClass) \
    extern "C" quint32 getQMPlay2ModuleAPIVersion() \
    { \
        return (QT_VERSION << 8) | QMPLAY2_MODULES_API_VERSION; \
    } \
    extern "C" Module *createQMPlay2ModuleInstance() \
    { \
        return new ModuleClass; \
    }

#endif // MODULE_HPP
