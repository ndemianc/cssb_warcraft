/**
* ��������� ������� � ���� WC3.
 
* @param name		��������
* @param cost		������
* @param onoff		��������/��������� �������
* @param messagebuy	��������� � �������	
* @return		An internal extra item ID, or -1 on failure.
 */
native wc3_register_extra_item(const name[], cost,onoff,messagebuy);
native wc3_buy_extra_item(idUser, idItem);
