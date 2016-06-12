#ifndef COLUMNS_H
#define COLUMNS_H

#define ID_COLUMN 10

#define USER_ID 0
#define USER_NAME 1
#define USER_ROLE 2

#define PRICETYPE_ID 0
#define PRICETYPE_NAME 1

#define EQUIP_ID 0
#define EQUIP_PROCESS 1
#define EQUIP_NAME 2
#define EQUIP_WIDTH 3
#define EQUIP_STATUS 4

#define MODE_ID 0
#define MODE_PRINTER_ID 1
#define MODE_X 2
#define MODE_Y 3
#define MODE_NAME 4
#define MODE_PRINTER_NAME 5

#define PASS_ID 0
#define PASS_MODE_ID 1
#define PASS_NAME 2
#define PASS_SPEED_BIDI 3
#define PASS_SPEED_UNI 4
#define PASS_MODE_NAME 5
#define PASS_PRINTER_NAME 6

#define PROFILE_ID 0
#define PROFILE_NAME 1

#define VAR_ID 0
#define VAR_NAME 1
#define VAR_INT 2
#define VAR_STRING 3
#define VAR_DATE_UPDATE 4
#define VAR_USER_NAME 5

#define CATEGORY_ID 0
#define CATEGORY_STOCK 1
#define CATEGORY_NAME 2
#define CATEGORY_PROCESS 3

#define MEDIA_ID 0
#define MEDIA_NAME 1
#define MEDIA_CATEGORY_NAME 2
#define MEDIA_CATEGORY_ID 3

#define CONTACT_ID 0
#define CONTACT_COMPANY_ID 1
#define CONTACT_NAME 2
#define CONTACT_SURNAME 3
#define CONTACT_EMAIL 4
#define CONTACT_TEL 5
#define CONTACT_COMPANY 6

#define COMPANY_ID 0
#define COMPANY_NAME 1
#define COMPANY_TYPE 2
#define COMPANY_ADDRESS 3
#define COMPANY_TEL 4
#define COMPANY_ACCOUNT 5
#define COMPANY_CREDIT 6
#define COMPANY_DEBT 7


#define MATERIAL_ID 0
#define MATERIAL_COMPANY_ID 1
#define MATERIAL_STOCK_ID 2
#define MATERIAL_STOCK_TYPE 3
#define MATERIAL_CATEGORY_NAME 4
#define MATERIAL_STOCK_NAME 5
#define MATERIAL_PARAM 6
#define MATERIAL_LENGTH 7
#define MATERIAL_AMOUNT 8
#define MATERIAL_SIZE 9
#define MATERIAL_INVOICE 10
#define MATERIAL_RECEIPT_DATE 11
#define MATERIAL_PRICE 12
#define MATERIAL_PAYMENT 13
#define MATERIAL_DEBT 14
#define MATERIAL_COMPANY_NAME 15

#define STOCK_ID 0
#define STOCK_CATEGORY_ID 1
#define STOCK_TYPE 2
#define STOCK_CATEGORY_NAME 3
#define STOCK_NAME 4
#define STOCK_PARAM 5
#define STOCK_AMOUNT 6
#define STOCK_SIZE 7

#define PRICE_ID 0
#define PRICE_PRICETYPE_ID 1
#define PRICE_CATEGORY_ID 2
#define PRICE_COMPANY_ID 3
#define PRICE_PROCESS 4
#define PRICE_CATEGORY_NAME 5
#define PRICE_STOCK_NAME 6
#define PRICE_PRICETYPE_NAME 7
#define PRICE_TOTAL 8
#define PRICE_DATE 9
#define PRICE_COMPANY_NAME 10

#define ORDER_ID 0
#define ORDER_COMPANY_ID 1
#define ORDER_CONTACT_ID 2
#define ORDER_EQUIP_ID 3
#define ORDER_CATEGORY_ID 4
#define ORDER_STOCK_ID 5
#define ORDER_CLIENT_ID 6
#define ORDER_DATE_START 7
#define ORDER_PROCESS 8
#define ORDER_EQUIP_NAME 9
#define ORDER_PRIORITY 10
#define ORDER_COMPANY_NAME 11
#define ORDER_CONTACT_NAME 12
#define ORDER_CATEGORY_NAME 13
#define ORDER_STOCK_NAME 14
#define ORDER_SIZE 15
#define ORDER_STATUS 16
#define ORDER_DATE_END 17
#define ORDER_CLIENT_NAME 18
#define ORDER_ISINVOICE 19
#define ORDER_ISRELEASE 20
#define ORDER_TOTAL 21
#define ORDER_TOTALPRINT 22
#define ORDER_TOTALPOSTPRINT 23
#define ORDER_PRICE 24

#define ORDER_DELIVERY_TYPE 1
#define ORDER_DELIVERY_DECLARATION 2
#define ORDER_DELIVERY_INFO 3
#define ORDER_PACK 4
#define ORDER_PACK_TYPE 5
#define ORDER_MARK 6
#define ORDER_STORY 7
#define ORDER_PROCESS_START 8
#define ORDER_PROCESS_END 9
#define ORDER_PROCESS_PREDICTTIME 10
#define ORDER_PROCESS_INFO 11
#define ORDER_WHO_START 12
#define ORDER_WHO_END 13

#define ORDERA_DATE_START 1
#define ORDERA_DATE_RELEASE 2
#define ORDERA_PROCESS 3
#define ORDERA_EQUIP_NAME 4
#define ORDERA_COMPANY_NAME 5
#define ORDERA_CONTACT_NAME 6
#define ORDERA_CATEGORY_NAME 7
#define ORDERA_STOCK_NAME 8
#define ORDERA_SIZE 9
#define ORDERA_MANAGER_NAME 10
#define ORDERA_STATUS 11
#define ORDERA_COMPANY_ID 12
#define ORDERA_DEBT 13

#define WORK_ID 0
#define WORK_ORDER_ID 1
#define WORK_STATUS 2
#define WORK_PROCESS 3
#define WORK_CATEGORY_NAME 4
#define WORK_STOCK_NAME 5
#define WORK_WIDTH 6
#define WORK_HEIGHT 7
#define WORK_AMOUNT 8
#define WORK_SIZE 9
#define WORK_TOTAL_PRINT 10
#define WORK_TOTAL_POSTPRINT 11

#define PROCESS_ID 0
#define PROCESS_TYPE 1
#define PROCESS_EQUIP_ID 2
#define PROCESS_PMODE_ID 3
#define PROCESS_PASS_ID 4
#define PROCESS_PROFILE_ID 5
#define PROCESS_MATERIAL 6
#define PROCESS_TOTAL 7
#define PROCESS_PRICE 8
#define PROCESS_TEXT_1 9
#define PROCESS_LVALUE_1 10
#define PROCESS_LVALUE_2 11
#define PROCESS_IVALUE_3 12
#define PROCESS_IVALUE_4 13
#define PROCESS_BVALUE 14
#define PROCESS_STATUS 15
#define PROCESS_WORK_ID 16

#define WRITEOFF_ID 0
#define WRITEOFF_STOCK_TYPE 1
#define WRITEOFF_DATE 2
#define WRITEOFF_ORDER_ID 3
#define WRITEOFF_PROCESS_TYPE 4
#define WRITEOFF_CATEGORY_NAME 5
#define WRITEOFF_STOCK_NAME 6
#define WRITEOFF_STOCK_PARAM 7
#define WRITEOFF_WORK_SIZE 8
#define WRITEOFF_AMOUNT 9
#define WRITEOFF_WASTE 10
#define WRITEOFF_OVERRUN 11
#define WRITEOFF_CLIENT_NAME 12
#define WRITEOFF_INFO 13
#define WRITEOFF_PROCESS_ID 14
#define WRITEOFF_PAYMENT_ID 15

#define PAYMENT_ID 0
#define PAYMENT_DATE 1
#define PAYMENT_TYPE 2
#define PAYMENT_TYPE_ID 3
#define PAYMENT_TOTAL 4
#define PAYMENT_INVOICE 5
#define PAYMENT_ISCACHE 6
#define PAYMENT_ISCREDIT 7
#define PAYMENT_CLIENT_NAME 8
#define PAYMENT_INFO 9

#endif // COLUMNS_H