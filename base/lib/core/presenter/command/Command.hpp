#pragma once
#include <QUndoCommand>
#include <QByteArray>
#include <QDataStream>
#include <QBuffer>
#include <chrono>

#define ISCORE_COMMAND \
    public: \
        static const char* className(); \
        static QString description(); \
    private:

namespace iscore
{
    /**
     * @brief The Command class
     *
     * The base of the command system in i-score
     * It is timestamped, because we can then compare between clients.
     *
     * Maybe the NetworkPlugin should replace the Command by a TimestampedCommand instead ?
     * What if other plug-ins also want to add functionality ?
     *
     * Note: for mergeWith put two timestamps, one for the initial command (5 sec) and one for each
     * new command merged.
     */
    class Command : public QUndoCommand
    {
        public:
            Command(QString parname, QString cmdname, QString text) :
                QUndoCommand {text},
                         m_name {cmdname},
            m_parentName {parname}
            {
            }

            QString name() const
            {
                return m_name;
            }
            QString parentName() const
            {
                return m_parentName;
            }

            // The realtime command editing mechanism
            // disables the merging when a command is applied.
            // Subclasses should implement this by returning
            // id() == -1 when canMerge() is false
            bool canMerge() const
            {
                return m_canMerge;
            }
            void enableMerging()
            {
                m_canMerge = true;
            }

            void disableMerging()
            {
                m_canMerge = false;
            }


        protected:
            quint32 timestamp() const
            {
                return m_timestamp.count();
            }
            void setTimestamp(quint32 stmp)
            {
                m_timestamp = std::chrono::duration<quint32> (stmp);
            }

        private:
            bool m_canMerge {false};
            const QString m_name;
            const QString m_parentName;

            //TODO check if this is UTC
            std::chrono::milliseconds m_timestamp
            {
                std::chrono::duration_cast<std::chrono::milliseconds>
                (
                    std::chrono::high_resolution_clock::now().time_since_epoch()
                )
            };
    };
}
