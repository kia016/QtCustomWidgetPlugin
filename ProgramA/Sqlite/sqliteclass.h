#ifndef SQLITECLASS_H
#define SQLITECLASS_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QElapsedTimer>


class SqliteClass : public QObject
{
    Q_OBJECT
public:
    explicit SqliteClass(QObject *parent = nullptr);
    ~SqliteClass();

    bool Open(QString file_path, QString file_name);
    bool TableExist(QString table);
    bool CreateTable(QString table, QString value);
    bool InsertDataQuery(QString table, QString column, QString value);
    bool InsertData(QString table, QString column, QString value);
    bool UpdateData(QString table, QStringList cols_vals, QStringList condition={});
    bool DeleteData(QString table, QStringList condition);
    QSqlQuery Select(QString table, QStringList select={}, QStringList condition={});
    int Count(QString table, QStringList condition={});

    void SetModel(QSqlQueryModel *model, QString query);

public slots:

    bool InsertMultiData(QString table, QString column, QStringList value);

signals:


private:
    QSqlDatabase db_;
};

#endif // SQLITECLASS_H
