#ifndef QDSPXMODEL_H
#define QDSPXMODEL_H

#include "QDspxTimeline.h"
#include "QDspxTrack.h"

namespace QDspx {

    // 文件的元信息，包括版本号、工程名、作者等
    struct DSCORE_API Metadata {
        QString version;
        QString name;
        QString author;
    };

    // 总线控制
    struct DSCORE_API Master {
        Control control;
    };

    // 工程可编辑区域
    struct DSCORE_API Content {
        Master master;
        Timeline timeline;
        QList<Track> tracks;

        // 不定长信息
        Extra extra;
        Workspace workspace;
    };

    // 工程
    struct DSCORE_API Model {
        Metadata metadata;
        Content content;

        // 不定长信息
        Workspace workspace;

        bool load(const QString &filename);
        bool save(const QString &filename) const;
    };

    QAS_JSON_NS(Metadata)
    QAS_JSON_NS(Master)
    QAS_JSON_NS(Content)
    QAS_JSON_NS(Model)

} // namespace QDspx

using QDspxModel = QDspx::Model;

#endif // QDSPXMODEL_H
