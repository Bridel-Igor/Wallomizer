#include <vector>

#include "IWindow.h"
#include "CollectionItem.h"
#include "CollectionManager.h"

class CollectionItemsFrame : public IWindow
{
private:
	/// Class manages resources (de)allocation for all CheckBox resources.
	class Resources
	{
	public:
		Resources();
		~Resources();

		static HBRUSH bkBrush;
	private:
		static unsigned char refCount;
	}resources;
public:
	CollectionItemsFrame(HWND hParent, CollectionManager* pCollectionManager, int x, int y, int width, int height);
	~CollectionItemsFrame();
	void updateCollectionItems();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void destroyCollectionItems();
	void updateScroll();

	int m_width, m_height;
	CollectionManager* m_pCollectionManager;
	Static stEmpty;

	std::vector<CollectionItem*> collectionItems;
	
	HDC hdc;
	PAINTSTRUCT ps;
	SCROLLINFO si;
	int yMinScroll = 0;
	int yCurrentScroll = 0;
	int yMaxScroll = 0;
	bool scrollBarIsVisible;
};