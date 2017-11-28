#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include <qnumeric.h>

class TimeStamp
{
public:
    inline bool isValid() const
    {
        return hasDts() || hasPts();
    }
    inline bool hasDts() const
    {
        return !qIsNaN(m_dts);
    }
    inline bool hasPts() const
    {
        return !qIsNaN(m_pts);
    }

    inline void setDts(double dts, double start_time = 0.0)
    {
        m_dts = dts - start_time;
    }
    inline void setPts(double pts, double start_time = 0.0)
    {
        m_pts = pts - start_time;
    }
    inline void setInvalid()
    {
        m_dts = m_pts = qQNaN();
    }

    inline double operator =(double t)
    {
        return (m_pts = m_dts = t);
    }
    inline void operator +=(double t)
    {
        if (hasDts())
            m_dts += t;
        if (hasPts())
            m_pts += t;
    }
    inline operator double() const
    {
        // XXX: Should it always return "m_dts"?
        if (m_dts >= 0.0)
            return m_dts;
        if (m_pts >= 0.0)
            return m_pts;
        return 0.0;
    }

    inline double pts() const
    {
        return m_pts;
    }
    inline double dts() const
    {
        return m_dts;
    }

private:
    double m_pts, m_dts;
};
#endif // TIMESTAMP_H
