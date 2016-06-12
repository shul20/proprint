#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QWidget>
#include "entity/product.h"
#include "entity/stock/stock.h"
#include "models/sortfiltermodel.h"
#include "models/stock/stockmodel.h"
#include "models/uniquefilter.h"

namespace Ui {
class ProductWidget;
}

class ProductWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProductWidget(QWidget *parent = 0);
    ~ProductWidget();

    void setup(bool isProduct, bool isMaterial, bool isParam,
               bool isMaterialLabel, bool isButton);

    void setRole(int role, QVariant match = QVariant());

    void setProduct(int productId);
    Product *getProduct() const;

    void setMaterial(int productId, int stockId);
    void setMaterial(int productId, const QString name);
    Stock *getMaterial() const;


    void setParam(int productId, int stockId);
    Stock *getParam() const;

    void disableFocus();

public slots:
    void setProcess(int processId);

private slots:
    void on_product_currentIndexChanged(int index);

    void on_material_currentIndexChanged(int index);

    void on_param_currentIndexChanged(int index);

    void on_addProduct_clicked();

    void productModelReset();
    void stockModelReset();

private:
    Ui::ProductWidget *ui;
    SortFilterModel *productF, *stockF, *paramF;
    UniqueFilter *materialF;

signals:
    void productChanged(Product *);
    void materialChanged(Stock *);
    void paramChanged(Stock *);
    void stockChanged();
};

#endif // PRODUCTWIDGET_H
