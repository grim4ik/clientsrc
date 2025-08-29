//----------------------------------------------------------------------
// ����:Edit�����ݽṹ�����㷨
// ����:lh 2004-07-12
// ����޸�����:
//----------------------------------------------------------------------
#pragma once

namespace GUI
{

class CEditArticle;
class CEditChar;
class CEditRow;
class CEditSentence;
class CEditObj;

// һ��,��Ϊһ������
class CEditRow
{
public:
	CEditRow();

	void	SetPos( int x, int y );
	void	PushUnit( CEditObj* pObj, CEditObj* pAtom );
	void	Clear();
	void	Render();
	DWORD	GetWordCount()					{ return _dwWordCount;				}

	CEditObj*	GetObj( DWORD dwIndex )		{ if( dwIndex<_units.size() ) return _units[dwIndex]; return nullptr;	}
	DWORD		GetObjNum()					{ return (DWORD)_units.size();		}

	DWORD		GetHeight()					{ return _dwHeight;					}
	DWORD		GetWidth()					{ return _dwWidth;					}

private:
	typedef std::vector<CEditObj*> units;
	units			_units;				// ��ʽ�������
	DWORD			_dwWordCount;
	units			_atoms;				// ԭʼ����ָ�룬�������ڴ棬���ڼ�����

	DWORD			_dwHeight;
	DWORD			_dwWidth;

};

class CEditTextObj;
class CEditControl;
class CEditStrategy;

class CEditParse
{
public:
	CEditParse()	{	_items.push_back( new CEditRow );	}

	// ��λ�ô��������,�ɹ����ز�����������ʧ�ܷ���-1
	int		Insert( DWORD nIndex, CEditObj* pObj )			{ return -1;			}

	// ɾ����ʼ����ֹ�Ŀ�
	bool	Del( DWORD nStart, DWORD nEnd );

	// �õ����д��Ĺ����Ϣ,�ڲ��Ƿ������һ���ı��������Լ���ɫ
	bool	GetPos( DWORD nIndex, int& x, int& y, DWORD& dwFont, DWORD& dwColor );

	// �õ�ѡ��������
	bool	GetSelectRect( DWORD dwStart, DWORD dwEnd, std::vector<POINT>& pt );

public:	
	// ���뵥Ԫ
	bool	InsertText( CEditTextObj* pText );
	bool	InsertControl( CEditControl* pControl );
	bool	InsertObj( CEditObj* pObj );

	// ������
	int		CursorText( CEditTextObj* pText );
	int		CursorControl( CEditControl* pControl );
	int		CursorObj( CEditControl* pControl );

private:
	typedef std::vector<CEditRow*> items;
	items		_items;					// �߼��ڴ棬������ʾ�������߼���

	DWORD		_dwRowLimit;			// ��������
	DWORD		_dwMaxWordLimit;		// �����������

};

// �㷨�����ڸ���ԭʼ�ڴ��������ʾ������
class CEditStrategy
{
public:
	CEditStrategy( CEditArticle* pActicle );
	~CEditStrategy();

	void	Init();
	bool	Append( CEditObj *pChar );
	void	Clear();
	void	Render();
	void	RefreshPos( int x, int y );

public:		// vistorģʽ�������ı������ƣ�ͼԪ�������͵Ľ���
	void	ParseText( CEditTextObj* pText );
	void	ParseControl( CEditControl* pControl );
	void	ParseObj( CEditObj* pObj );

private:
	CEditRow*	_AppendToBackRow( CEditObj *pChar );

private:
	CEditArticle*	_pActicle;	

	typedef std::vector<CEditRow*> items;
	items			_items;			// �߼��ڴ棬������ʾ�������߼���

};

inline void CEditStrategy::ParseObj( CEditObj* pObj )
{
	// ���ɺϲ���ͼԪ
	CEditRow* pRow = _AppendToBackRow( pObj );
	pRow->PushUnit( pObj, pObj );
}

inline void CEditStrategy::ParseControl( CEditControl* pControl )
{
	_items.push_back( new CEditRow );	
}

}
