//----------------------------------------------------------------------
// ����:���ڹ�����ͬ�ı����ڴ�
// ����:lh 2005-03-15
// ����޸�����:
//----------------------------------------------------------------------
#pragma once

namespace GUI
{
class CForm;

// ����ͬһ���¡�ı���,��ɾ��ʱ����������ɾ��,��������,�´ο�¡ʱ��ֱ��ʹ�� 
class CCloneForm
{
public:
	CCloneForm();
	~CCloneForm();

	void	SetSample( CForm* frm )	{ _pSample = frm;	}

	CForm*	Clone();
	bool	Release( CForm* p );

private:
	CForm*	_pSample;		// ��������

	typedef std::vector<CForm*>	vfrm;
	vfrm	_vfrm;

    int     _nCount;		// ��Ч�ı�������

};

// ��ͬһ�������,�Զ��õ����صı���,���û�����صı���,���Զ���¡һ��
class CHideForm
{
public:
	void	Init( CForm* frm )		{ _vfrm.push_back( frm );	}

	CForm*	GetHide();
	void	CloseAll();		// �ر����б���

	CForm*  GetForm( int n )		{ return _vfrm[n];			}
	int		GetCount()				{ return (int)_vfrm.size();	}

	CForm*  operator[]( int n )		{ return _vfrm[n];			}

private:
	typedef std::vector<CForm*>	vfrm;
	vfrm	_vfrm;

};

}

