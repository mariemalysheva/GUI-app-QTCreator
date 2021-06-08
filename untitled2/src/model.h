#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>
#include <QList>

struct Item{
    int anime_id;
    QString name;
    QString genre;
    QString type;
    QString episodes;
    double rating;
    int members;

};

class model : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit model(QObject *parent = nullptr){

    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void appendRow(QList<QString> newRow);


    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool rowIsEmpty(int i) const;
    bool isEmpty() const;


    bool genreMatches(QString& genreString, int row);
    bool searchMatches(QString& searchString, int row);
    void search(QString& searchString,  QString& genreString, model* searchResults);
    static bool rComp(const Item &a1, const Item &a2);
    QList<Item> sortByRating();
    QList<Item> getItemList();
    int modelSorted();
    void fillModelWithData( QString path);
    void saveModelAsFile(QString path);
    void deleteRow(int idx);
    void clear();
    void deleteFromAll(model* searchResults, int pos1);
    QList<QString> idList();











private:
    QList<QList<QString>> mydata;
    QList<QString> headers = {"anime_id", "name", "genre", "type", "episodes", "rating", "members"};

};

#endif // MODEL_H
