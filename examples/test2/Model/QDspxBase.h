#ifndef QDSPXBASE_H
#define QDSPXBASE_H

#include <QJsonObject>
#include <QMap>

#include "DsCoreGlobal.h"
#include "qas.h"

namespace QDspx {

    // 其他参数，不定长
    using Extra = QMap<QString, QJsonObject>;

    // 状态信息，不定长
    using Workspace = QMap<QString, QJsonObject>;

    // 外部资源信息，不定长
    using SourceInfo = QMap<QString, QJsonObject>;

    // 主控
    struct DSCORE_API Control {
        double gain;
        bool mute;

        // 构造器
        Control() : Control(0, false) {};

        Control(double gain, bool mute) : gain(gain), mute(mute) {};
    };

    // 音轨主控
    struct DSCORE_API TrackControl : public Control {
        double pan;
        bool solo;

        // 构造器
        TrackControl() : TrackControl(0, false) {};

        TrackControl(double pan, bool solo) : pan(pan), solo(solo) {};

        TrackControl(double gain, double pan, bool mute, bool solo)
                : Control(gain, mute), pan(pan), solo(solo) {};
    };

    // 泛型点
    template<class T>
    struct DSCORE_API Point {
        T x;
        T y;

        // 构造器
        Point() : Point(0, 0) {};

        Point(T x, T y) : x(x), y(y) {};
    };

    using IntPoint = Point<int>;

    using DoublePoint = Point<double>;

    // 控制点
    struct DSCORE_API AnchorPoint : public IntPoint {
        enum Interpolation {
            QAS_ATTRIBUTE("none")
            None,

            QAS_ATTRIBUTE("linear")
            Linear,

            QAS_ATTRIBUTE("hermite")
            Hermite,
        };

        Interpolation interp;

        // 构造器
        AnchorPoint() : interp(Linear) {};

        AnchorPoint(int x, int y, Interpolation i = Linear) : IntPoint(x, y), interp(i) {};
    };

} // namespace QDspx

QAS_JSON_DECLARE(QDspx::Control)

QAS_JSON_DECLARE(QDspx::TrackControl)

QAS_JSON_DECLARE(QDspx::IntPoint)

QAS_JSON_DECLARE(QDspx::DoublePoint)

QAS_JSON_DECLARE(QDspx::AnchorPoint)

QAS_ENUM_DECLARE(QDspx::AnchorPoint::Interpolation)

#endif // QDSPXBASE_H
