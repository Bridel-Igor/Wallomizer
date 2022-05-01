#include "CollectionItemsFrame.h"
#include "WindowStyles.h"

CollectionItemsFrame::CollectionItemsFrame(HWND hParent, CollectionManager* pCollectionManager, int x, int y, int width, int height) :
	IWindow("", "Collection Items Frame Class", WS_CHILD | WS_BORDER | WS_VSCROLL, NULL, x, y, width, height, hParent),
	m_width(width), m_height(height),
	m_pCollectionManager(pCollectionManager),
	stEmpty(hWnd(), "Collection list is empty. Click \"Add collection..\" button to add one.", 5, 0, 480, 20)
{
	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	updateCollectionItems();
}

CollectionItemsFrame::~CollectionItemsFrame()
{
	destroyCollectionItems();
}

void CollectionItemsFrame::updateCollectionItems()
{
	ShowWindow(stEmpty.hWnd(), SW_HIDE);

	for (size_t i = m_pCollectionManager->m_pCollections.size(); i < collectionItems.size(); i++) //deleting excess items
	{
		delete collectionItems.back();
		collectionItems.pop_back();
	}

	for (size_t i = 0; i < collectionItems.size(); i++) // updating those which won't be created
		collectionItems[i]->updateInfo(m_pCollectionManager->m_pCollections[i]);

	for (size_t i = collectionItems.size(); i < m_pCollectionManager->m_pCollections.size(); i++) // creation
		if (m_pCollectionManager->m_pCollections[i] != nullptr)
			collectionItems.push_back(new CollectionItem(hWnd(), 0, (int)(i * (CollectionItem::height + 1)), m_width, m_pCollectionManager->m_pCollections[i], WindowStyles::mainFont));

	updateScroll();
	for (auto p : collectionItems) // placing according to the scrollbar
		p->reposition(yCurrentScroll, scrollBarIsVisible);

	if (collectionItems.size() == 0)
		ShowWindow(stEmpty.hWnd(), SW_SHOW);

	InvalidateRect(hWnd(), NULL, FALSE);
}

void CollectionItemsFrame::destroyCollectionItems()
{
	for (auto p : collectionItems)
		delete p;
	collectionItems.clear();
	updateScroll();
}

void CollectionItemsFrame::updateScroll()
{
	int itemListHeight = (int)collectionItems.size() * (CollectionItem::height + 1);
	yMaxScroll = max(itemListHeight - m_height, 0);
	yCurrentScroll = min(yCurrentScroll, yMaxScroll);
	yCurrentScroll = yCurrentScroll < 0 ? 0 : yCurrentScroll;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = yMinScroll;
	si.nMax = itemListHeight;
	si.nPage = m_height;
	si.nPos = yCurrentScroll;

	scrollBarIsVisible = itemListHeight <= m_height ? FALSE : TRUE;
	SetScrollInfo(hWnd(), SB_VERT, &si, TRUE);
	ShowScrollBar(hWnd(), SB_VERT, scrollBarIsVisible);
	EnableScrollBar(hWnd(), SB_VERT, itemListHeight <= m_height ? ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
}

LRESULT CollectionItemsFrame::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SHOWWINDOW:
	{
		if (wParam == TRUE)
			InvalidateRect(this->hWnd(), NULL, FALSE);
	}
	return 0;

	case WM_PAINT:
	{
		hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, WindowStyles::collFrameBkBrush);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		for (int i = 0; i < collectionItems.size(); i++)
			if (collectionItems[i]->draw(pDIS))
				return TRUE;
	}
	return 0;

	case WM_COMMAND:
	{
		for (unsigned int i = 0; i < collectionItems.size(); i++)
		{
			if (collectionItems[i]->btnSettings.isClicked(wParam))
			{
				m_pCollectionManager->m_pCollections[i]->openCollectionSettingsWindow();
				return 0;
			}
			if (collectionItems[i]->btnDelete.isClicked(wParam))
			{
				m_pCollectionManager->eraseCollection(i);
				updateCollectionItems();
				InvalidateRect(this->hWnd(), nullptr, TRUE);
				return 0;
			}
			if (collectionItems[i]->chboEnabled.isClicked(wParam))
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					collectionItems[i]->chboEnabled.click();
					m_pCollectionManager->m_pCollections[i]->setEnabled(collectionItems[i]->chboEnabled.isChecked());
					m_pCollectionManager->reloadSettings();
					return 0;
				}
			}
		}
	}
	return 0;

	case WM_MOUSEWHEEL:
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			SendMessage(this->hWnd(), WM_VSCROLL, SB_LINEUP, 0L);
		else
			SendMessage(this->hWnd(), WM_VSCROLL, SB_LINEDOWN, 0L);
	}
	return 0;

	case WM_VSCROLL:
	{
		int yDelta;
		int yNewPos;

		switch (LOWORD(wParam))
		{
		case SB_PAGEUP:
			yNewPos = yCurrentScroll - 60;
			break;
		case SB_PAGEDOWN:
			yNewPos = yCurrentScroll + 60;
			break;
		case SB_LINEUP:
			yNewPos = yCurrentScroll - 10;
			break;
		case SB_LINEDOWN:
			yNewPos = yCurrentScroll + 10;
			break;
		case SB_THUMBTRACK:
			yNewPos = HIWORD(wParam);
			break;
		default:
			yNewPos = yCurrentScroll;
		}

		yNewPos = max(0, yNewPos);
		yNewPos = min(yMaxScroll, yNewPos);

		if (yNewPos == yCurrentScroll)
			break;

		yDelta = yNewPos - yCurrentScroll;
		yCurrentScroll = yNewPos;

		updateScroll();
		for (auto p : collectionItems) // placing according to the scrollbar
			p->reposition(yCurrentScroll, scrollBarIsVisible);

		ScrollWindowEx(this->hWnd(), 0, -yDelta, (CONST RECT*) NULL, (CONST RECT*) NULL, (HRGN)NULL, (PRECT)NULL, SW_INVALIDATE);
		UpdateWindow(this->hWnd());
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;

		for (int i = 0; i < collectionItems.size(); i++)
			if (hWndStatic == collectionItems[i]->stNumber.hWnd())
			{
				if (collectionItems[i]->chboEnabled.isChecked())
					SetTextColor(hdcStatic, WindowStyles::collItemFontColor);
				else
					SetTextColor(hdcStatic, RGB(80, 80, 80));
				SetBkColor(hdcStatic, WindowStyles::collItemBkColor);
				return (LRESULT)WindowStyles::collItemBkBrush;
			}

		if (hWndStatic == stEmpty.hWnd())
		{
			SetTextColor(hdcStatic, WindowStyles::collItemFontColor);
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LRESULT)WindowStyles::collFrameBkBrush;
		}
		SetTextColor(hdcStatic, WindowStyles::collItemFontColor);
		SetBkColor(hdcStatic, WindowStyles::collItemBkColor);
		return (LRESULT)WindowStyles::collItemBkBrush;
	}
	return 0;

	case WM_SETCURSOR:
	{
		for (int i = 0; i < collectionItems.size(); i++)
			collectionItems[i]->mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}