#pragma once

#include "Scene.h"
#include "uiguidata.h"
#include "NetProtocol.h"

class CSceneObj;
namespace GUI
{
    class CForm;
    class C3DCompent;
    class CGuiData;
    class CLabelEx;
    class CEdit;
    class CTextButton;
    class CList;
    class CMemo;
}

/* extern char autoLoginChar[32] ;
extern bool canAutoLoginChar; */
/**
 * ժҪ��������Ҫ������½�����е�ѡ���ɫ����,��������������GUI����.
 * ����������̳���CGameScene��.
 *
 * @author: Michael Chen
 * @date: 2005-04-26
 */
class CSelectChaScene : public CGameScene
{
public:
    CSelectChaScene(stSceneInitParam& param);
    ~CSelectChaScene();

    virtual void        LoadingCall();          // ��װ��loading��,ˢ��
    virtual void		SetMainCha(int nChaID);

    static CSelectChaScene& GetCurrScene();

    //ɾ����ǰѡ�еĽ�ɫ
    void                DelCurrentSelCha();
    //��ʾ���н�ɫ
    bool                SelectCharacters(NetChaBehave* chabehave, int num);
    //������ɫ
    bool                CreateCha(const std::string& sName, int nChaIndex, stNetChangeChaPart* part);

    void                SelectChaError( int error_no, const char* error_info );

    //�����������ɾ����ɫ��Ϣ
    void SendDelChaToServer(const char szPassword2[]);

	// ���°�ť״̬
    void UpdateButton();

	// ��ý�ɫ����
	int GetChaCount();

	void ShowWelcomeNotice(bool bShow = true);

protected:
    //������غ���
    virtual void		_FrameMove(DWORD dwTimeParam);
    virtual void		_Render();

    virtual bool		_MouseButtonDown(int nButton);
    virtual bool		_MouseButtonDB(int nButton);
	virtual void		_KeyDownEvent( int key );


    virtual bool		_Init();	
    virtual bool		_Clear();
    virtual void        _RenderUI();

    //UI��غ���
    bool                _InitUI();
    static void         _SelChaFrmMouseEvent(CCompent *pSender, int nMsgType, 
                                             int x, int y, DWORD dwKey);

//	static void         _CheckFrmMouseEvent(CCompent *pSender, int nMsgType, 
//                                           int x, int y, DWORD dwKey);

    static void         _evtCreateDoublePwdEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey);		// ѯ���Ƿ�Ҫ������������

    static void         _evtWelcomeNoticeEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey);		// ��ӭ���� �¼�����

    static void         _evtCreateOKNoticeEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey);		// �״δ�����ɫ�ɹ���ʾ���� �¼�����

    static void         _evtChaNameAlterMouseEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey);		// �״δ�����ɫ�ɹ���ʾ���� �¼�����

	//~ �߼�����: ============================================================


    //����������Ϳ�ʼ��Ϸ��Ϣ
    void SendBeginPlayToServer();

    void SetChaDark(CCharacter* pCha);

private:
    struct ChaFont
    {
		ChaFont() : pCha(nullptr), iLevel(-1), sProfession(""), iPos(-1), iFontX(-1), iFontY(-1) {}
        CCharacter * pCha;
        int          iLevel;
        std::string       sProfession;
        int          iPos;
		int			 iFontX;
		int			 iFontY;
    };
    typedef std::vector<ChaFont*> ChaFontPtrContainer;
    typedef ChaFontPtrContainer::iterator ChaFontPtrContainerIter;

    ChaFontPtrContainer m_CharactorPtrs;        //Characters in the scene (including fonts)
    std::vector<int>         m_XPositions;           //Scene X coordinates of the three characters in the scene
    std::vector<int>         m_YPositions;           //Y coordinates of the three characters in the scene
    std::vector<int>         m_ZPositions;
    std::vector<int>         m_Yaws;                 //The rotation of the characters in the scene
    std::vector<int>         m_FreePositions;        //The index of the empty position of the scene
    BYTE                m_ChaColors[3][3];      //�������ɫ
	int					m_nChaRotates[3];       //�����ƫ�ƽǶ�(-180~+180)


    //������Ⱦ�Ͷ�����ص�
    BYTE            _loadtex_flag;          //���泡����Ⱦ�Ͷ���������
    BYTE            _loadmesh_flag;         //���泡����Ⱦ�Ͷ���������

    CSceneObj*	    pObj;                   //��������
    lwIPrimitive*   pAure[3];


    //UI
    CForm*			frmSelectCha; 
    CTextButton*    btnDel;
    CTextButton*    btnYes;
    CTextButton*    btnCreate;
    CTextButton*    btnExit;
	CTextButton*    btnAlter;

    CForm*          frmCheck;

    //�߼�
    int             m_nCurChaIndex;

    //
    bool            m_isInit;

	bool			m_isCreateCha;

	CForm*			frmWelcomeNotice;	// ���延ӭ����  �ý�����ڵ�ǰ�ʺ����޽�ɫʱ����
	CForm*			frmCreateOKNotice;	// �����״δ�����ɫ�ɹ���ʾ����  �ý�����ڸ��ʺ������һ����ɫ�Ĵ������̺���ʾ
	CForm*			frmChaNameAlter;	// �����������  �ý������̨��汾��Ч�������ñ��ݹ���
};
