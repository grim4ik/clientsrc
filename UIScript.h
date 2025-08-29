//----------------------------------------------------------------------
// ����:UI�ű���
// ����:lh 2004-10-26
// ��;:������,����֧�����������ܹ������ⲿ�ű�
//   ע:Ŀǰ�����ڳ����ж�̬ɾ��
// ����޸�����:
//----------------------------------------------------------------------
#pragma once
#include "uiguidata.h"

namespace GUI
{

template <class T>
class UIScript
{
public:
	T*		GetObj( unsigned int nIndex )	{ if(nIndex>=list.size()) return nullptr;	return list[nIndex];	}
	int		AddObj( T* p )					{ list.push_back(p); return (int)(list.size()-1);				}

private:
	typedef std::vector<T*> vt;
	vt		list;

};

}
