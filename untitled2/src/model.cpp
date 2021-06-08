#include "model.h"
#include <QSet>
#include <algorithm>
#include <QString>
#include <QFile>
#include <QTextStream>

void model::fillModelWithData( QString path)
{
    QFile inputFile(path);
    inputFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream inputStream(&inputFile);

    inputStream.readLine();


    while(!inputStream.atEnd())
    {
        QString line = inputStream.readLine();
        QList<QString> dataRow;
        QList<QString> raw = line.split(",");

        for (int i=0; i<2; ++i){
            dataRow.append(raw[i]);
        }
        if (raw.size()>7){
        QString genre;
        for(int i=2; i<(raw.size()-4); ++i){
            genre+=(raw[i]+",");
        }
        genre.chop(2);
        genre = genre.mid(1);

        dataRow.append(genre);
        }
        else
            dataRow.append(raw[2]);
        for (int i=(raw.size()-4); i<raw.size(); ++i){
            dataRow.append(raw[i]);
        }

        appendRow(dataRow);
    }
    inputFile.close();

}

int model::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return mydata.size();
}


void model::saveModelAsFile(QString path)
{
    QFile outFile(path);
    outFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&outFile);


    for (int j = 0; j < columnCount(); ++j)
    {
        out << headers.at(j);
        if (j != columnCount() - 1)
        {
            out << ",";
        }
    }
    out << "\n";

    for (int i = 0; i < rowCount(); ++i)
    {
        for (int j = 0; j < columnCount(); ++j)
        {
            QModelIndex idx = index(i, j);
            if (j==2 and mydata.at(i).at(j).contains(","))
                out<<'"';
            out << data(idx).toString();
            if (j==2 and mydata.at(i).at(j).contains(","))
                out<<'"';
            if (j != columnCount() - 1)
            {
                out << ",";
            }
        }
        out << "\n";
    }
    outFile.close();
}


int model::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (mydata.empty())
    {
        return 0;
    }
    return mydata[0].size();
}


QVariant model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        int row = index.row();
        int column = index.column();

        return mydata.at(row).at(column);
    }
    if (index.column() != 1 && index.column() != 2 && role==Qt::TextAlignmentRole)
        return  Qt::AlignCenter;
    return QVariant();
}



bool model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {

        mydata[index.row()][index.column()] = value.toString();

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

QVariant model::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
             return QVariant();
    if (orientation==Qt::Horizontal)
        return headers[section];
    else
        return section+1;

}

Qt::ItemFlags model::flags(const QModelIndex &index) const
{
    return Qt::ItemIsUserCheckable | QAbstractTableModel::flags(index);
}

void model::appendRow(QList<QString> newRow)
{

    bool columnFlag = false;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    if (mydata.isEmpty())
    {
        columnFlag = true;
        beginInsertColumns(QModelIndex(), 0, newRow.size() - 1);
    }
    mydata.append(newRow);


    if (columnFlag)
    {
        endInsertColumns();
    }
    endInsertRows();
}


bool model::rowIsEmpty(int i) const{
    if(mydata.at(i).isEmpty())
        return true;
    return false;
}

bool model::isEmpty() const{
    return mydata.isEmpty();
}

bool model::searchMatches(QString& searchString, int row){

    QString cell = mydata.at(row).at(1);

    if(cell.contains(searchString, Qt::CaseInsensitive))
        return true;

    else
        return false;
}


bool model::genreMatches(QString& genreString, int row){

    QList<QString> chosen = genreString.split(", ");
    QString cell = mydata.at(row).at(2);
    bool con=true;
    for(QString& item: chosen){
        if(!cell.contains(item, Qt::CaseInsensitive))
            con= false;
    }
    if (con)
        return true;
    else
        return false;
}


void model::search(QString& searchString, QString& genreString,  model* searchResults){
    for (int i=0; i<rowCount(); ++i){
        if (genreMatches(genreString, i) and searchMatches(searchString, i))
            searchResults->appendRow(mydata.at(i));
    }
}




QList<Item> model::getItemList(){
    QList<Item> items;

    for (int i=0; i<rowCount(); ++i){
        Item item;
        item.anime_id = mydata[i][0].toInt();
        item.name = mydata[i][1];
        item.genre = mydata[i][2];
        item.type = mydata[i][3];
        item.episodes = mydata[i][4];
        item.rating = mydata[i][5].toDouble();
        item.members = mydata[i][6].toInt();
        items.append(item);
    }
    return items;
}




bool model::rComp(const Item &a1, const Item &a2)
{
    return a1.rating> a2.rating;
}

QList<Item> model::sortByRating(){
    QList<Item> items = getItemList();
    std::sort(items.begin(), items.end(), rComp);
    return items;
}

int model::modelSorted(){

    QList<Item> items = sortByRating();
    clear();
    for (int i=0; i<items.size(); ++i){
        QList<QString> row;
        QString s_id = QString::number(items[i].anime_id);
        row.append(s_id);
        row.append(items[i].name);
        row.append(items[i].genre);
        row.append(items[i].type);
        row.append(items[i].episodes);
        QString s_r = QString::number(items[i].rating, 'f', 2);
        row.append(s_r);
        QString s_m = QString::number(items[i].members);
        row.append(s_m);
        appendRow(row);
    }
    return items.size();
}


void model::deleteRow(int idx)
{
    beginRemoveRows(QModelIndex(), idx, idx);
    mydata.removeAt(idx);
    endRemoveRows();
}

void model::clear(){
    beginResetModel();
    mydata.clear();
    endResetModel();

}

QList<QString> model::idList(){
    QList<QString> ids;
    for (int i=0; i<rowCount(); ++i){
        ids.append(mydata.at(i).at(0));
    }
    return ids;
}

void model::deleteFromAll(model* searchResults, int pos1){
    QString id = searchResults->mydata.at(pos1).at(0);
    QList<QString> ids = idList();
    int pos2 = ids.indexOf(id);
    deleteRow(pos2);
}







