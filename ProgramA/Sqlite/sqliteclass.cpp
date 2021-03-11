#include "sqliteclass.h"

SqliteClass::SqliteClass(QObject *parent) : QObject(parent)
{

}

SqliteClass::~SqliteClass()
{
    if (db_.isOpen()) {
        db_.close();
    }
}

bool SqliteClass::Open(QString file_path, QString file_name)
{
    QDir dir(file_path);
    if(!dir.exists()) {
        if (!dir.mkpath(file_path)) {
            QMessageBox::critical(nullptr, tr("File path error"), tr("File path not found!"));
            return false;
        }
    }
    db_ = QSqlDatabase::addDatabase("QSQLITE", file_name);
    db_.setDatabaseName(file_path + "/" + file_name);
    if (!db_.open()) {
        QMessageBox::critical(nullptr, tr("Database error"), tr(db_.lastError().text().toStdString().c_str()));
        return false;
    }
    return true;
}

bool SqliteClass::TableExist(QString table)
{
    QSqlQuery query(db_);
    query.exec(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(table));
    if (!query.next()) {
        return false;
    }
    return true;
}

bool SqliteClass::CreateTable(QString table, QString value)
{
    QSqlQuery query(db_);
    if (query.exec(QString("CREATE TABLE %1 (%2)").arg(table, value))) {
        return true;
    } else {
        qDebug() << query.lastError().text();
        return false;
    }
}

bool SqliteClass::InsertDataQuery(QString table, QString column, QString value)
{
    QSqlQuery query(db_);
    QStringList list = column.split(", ");
    for(int i = 0; i < list.size(); i++) {
        list[i] = ":" + list[i];
    }
    QString target = list.join(", ");
    query.prepare(QString("INSERT INTO %1 (%2) VALUES (%3)").arg(table, column, target));
    QStringList value_list = value.split(", ");
    for(int i = 0; i < value_list.size(); i++) {
        query.bindValue(list[i], value_list[i]);
    }
    if (query.exec()) {
        return true;
    } else {
        qDebug() << query.lastError().text();
        return false;
    }
}

bool SqliteClass::InsertData(QString table, QString column, QString value)
{
//    db_.transaction();
    QSqlQuery query(db_);
    if (query.exec(QString("INSERT INTO %1 (%2) VALUES (%3)").arg(table, column, value))) {
        return true;
    } else {
        qDebug() << query.lastError().text();
        return false;
    }
    db_.commit();
}

bool SqliteClass::InsertMultiData(QString table, QString column, QStringList value)
{
    QElapsedTimer t;
    t.start();
    db_.transaction();
    QSqlQuery query(db_);
    for(int i = 0; i < value.size(); i++) {
        value[i] = "(" + value[i] + ")";
    }
    QString target = value.join(",");
    if (query.exec(QString("INSERT INTO %1 (%2) VALUES %3").arg(table, column, target))) {
        db_.commit();
        qDebug() << "insert database:" << t.elapsed();
        return true;
    } else {
        qDebug() << query.lastError().text();
        db_.commit();
        qDebug() << "insert database:" << t.elapsed();
        return false;
    }
}

bool SqliteClass::UpdateData(QString table, QStringList cols_vals, QStringList condition)
{
    QSqlQuery query(db_);
    QString target;
    QString option;
    target = cols_vals.join(", ");
    if (condition.isEmpty()) {
        option = "";
    } else {
        option = "WHERE " + condition.join(" AND ");
    }
    query.prepare(QString("UPDATE %1 SET %2 %3").arg(table, target, option));
    if (query.exec()) {
        db_.commit();
        return true;
    } else {
        qDebug() << query.lastError().text();
        return false;
    }
}

bool SqliteClass::DeleteData(QString table, QStringList condition)
{
    QSqlQuery query(db_);
    QString option = condition.join(" AND ");
    query.prepare(QString("DELETE FROM %1 WHERE %2").arg(table, option));
    if (query.exec()) {
        db_.commit();
        return true;
    } else {
        qDebug() << query.lastError().text();
        return false;
    }
}

QSqlQuery SqliteClass::Select(QString table, QStringList select, QStringList condition)
{
    QSqlQuery query(db_);
    QString column;
    QString option;
    if (select.isEmpty()) {
        column = "*";
    } else {
        column = select.join(", ");
    }
    if (condition.isEmpty()) {
        option = "";
    } else {
        option = "WHERE " + condition.join(" AND ");
    }
    query.prepare(QString("SELECT %1 FROM %2 %3").arg(column, table, option));
    query.exec();
    return query;
}

int SqliteClass::Count(QString table, QStringList condition)
{
    QSqlQuery query(db_);
    QString option;
    if (condition.isEmpty()) {
        option = "";
    } else {
        option = "WHERE " + condition.join(" AND ");
    }
    query.prepare(QString("SELECT "
                          "COUNT(*) "
                          "FROM %1 "
                          "%2").arg(table, option));
    query.exec();
    if (query.next()) {
        return query.value(0).toInt();
    } else {
        return 0;
    }
}

void SqliteClass::SetModel(QSqlQueryModel *model, QString query)
{
    model->setQuery(query, db_);
}
