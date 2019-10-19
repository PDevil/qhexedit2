#pragma once

#include "chunks.h"

#include <qabstractscrollarea.h>
#include <qpen.h>

enum class BreakpointType : uint8_t
{
	None = 0,
	Read,
	Write,
	ReadWrite
};

class QHexEdit2 : public QAbstractScrollArea
{
	Q_OBJECT;

public:
	/*!
		Property address area switch the address area on or off. Set addressArea true
		(show it), false (hide it).
	*/
	Q_PROPERTY(bool addressArea READ addressArea WRITE setAddressArea);
	bool _addressArea;
	bool addressArea();
	void setAddressArea(bool addressArea);

	/*!
		Show hex area or no.
	*/
	Q_PROPERTY(bool hexArea READ hexArea WRITE setHexArea);
	bool _hexArea;
	bool hexArea();
	void setHexArea(bool hexArea);

	/*!
		Property address area color sets (setAddressAreaColor()) the background
		color of address areas. You can also read the color (addressAreaColor()).
	*/
	Q_PROPERTY(QColor addressAreaColor READ addressAreaColor WRITE setAddressAreaColor);
	QColor _addressAreaColor;
	QColor addressAreaColor();
	void setAddressAreaColor(const QColor& color);

	/*!
		Property address area font color sets (setAddressAreaFontColor()) the font
		color of address areas. You can also read the color (addressAreaFontColor()).
	*/
	Q_PROPERTY(QColor addressAreaFontColor READ addressAreaFontColor WRITE setAddressAreaFontColor);
	QColor _addressAreaFontColor;
	QColor addressAreaFontColor();
	void setAddressAreaFontColor(const QColor& color);

	/*!
		Property addressOffset is added to the Numbers of the Address Area.
		A offset in the address area (left side) is sometimes useful, whe you show
		only a segment of a complete memory picture. With setAddressOffset() you set
		this property - with addressOffset() you get the current value.
	*/
	Q_PROPERTY(qint64 addressOffset READ addressOffset WRITE setAddressOffset);
	qint64 _addressOffset;
	qint64 addressOffset();
	void setAddressOffset(qint64 addressArea);

	/*!
		Set and get the minimum width of the address area, width in characters.
		Basically how many zeroes prefixed we want at max.
	*/
	Q_PROPERTY(int addressWidth READ addressWidth WRITE setAddressWidth);
	int _addressWidth;
	int addressWidth();
	void setAddressWidth(int addressWidth);

	/*!
		Switch the ascii area on (true, show it) or off (false, hide it).
	*/
	Q_PROPERTY(bool asciiArea READ asciiArea WRITE setAsciiArea);
	bool _asciiArea;
	bool asciiArea();
	void setAsciiArea(bool asciiArea);

	/*!
		Set ascii area background color
	 */
	Q_PROPERTY(QColor asciiAreaBackgroundColor READ asciiAreaBackgroundColor WRITE setAsciiAreaBackgroundColor);
	QColor _asciiAreaBackgroundColor;
	QColor asciiAreaBackgroundColor();
	void setAsciiAreaBackgroundColor(QColor color);

	/*!
		Set and get bytes number per line.
	*/
	Q_PROPERTY(int bytesPerLine READ bytesPerLine WRITE setBytesPerLine);
	int _bytesPerLine;
	int bytesPerLine();
	void setBytesPerLine(int count);

	/*!
		Property cursorPosition sets or gets the position of the editor cursor
		in QHexEdit. Every byte in data has two cursor positions: the lower and upper
		Nibble. Maximum cursor position is factor two of data.size().
	*/
	Q_PROPERTY(qint64 cursorPosition READ cursorPosition WRITE setCursorPosition);
	qint64 _cursorPosition;
	qint64 cursorPosition();
	void setCursorPosition(qint64 position);

	/*!
		Property data holds the content of QHexEdit. Call setData() to set the
		content of QHexEdit, data() returns the actual content. When calling setData()
		with a QByteArray as argument, QHexEdit creates a internal copy of the data
		If you want to edit big files please use setData(), based on QIODevice.
	*/
	Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged);
	QByteArray _data;
	QByteArray data();
	void setData(const QByteArray& ba);

	/*!
		That property defines if the hex values looks as a-f if the value is false(default)
		or A-F if value is true.
	*/
	Q_PROPERTY(bool hexCaps READ hexCaps WRITE setHexCaps);
	bool _hexCaps;
	void setHexCaps(const bool isCaps);
	bool hexCaps();

	/*!
		Property defines the dynamic calculation of bytesPerLine parameter depends of width of widget.
		set this property true to avoid horizontal scrollbars and show the maximal possible data. defalut value is false*/
	Q_PROPERTY(bool dynamicBytesPerLine READ dynamicBytesPerLine WRITE setDynamicBytesPerLine);
	bool _dynamicBytesPerLine;
	void setDynamicBytesPerLine(const bool isDynamic);
	bool dynamicBytesPerLine();

	/*!
		Set breakpoint field color
	*/
	Q_PROPERTY(QColor breakpointFieldColor READ breakpointFieldColor WRITE setBreakpointFieldColor);
	QColor _breakpointFieldColor;
	QColor breakpointFieldColor();
	void setBreakpointFieldColor(QColor color);

	/*!
		Set hex area field color
	*/
	Q_PROPERTY(QColor hexFieldColor READ hexFieldColor WRITE setHexFieldColor);
	QColor _hexFieldColor;
	QColor hexFieldColor();
	void setHexFieldColor(QColor color);

	/*!
		Switch the highlighting feature on or of: true (show it), false (hide it).
	*/
	//Q_PROPERTY(bool highlighting READ highlighting WRITE setHighlighting);
	//bool _highlighting;
	//bool highlighting();
	//void setHighlighting(bool mode);

	/*!
		Property highlighting color sets (setHighlightingColor()) the background
		color of highlighted text areas. You can also read the color
		(highlightingColor()).
	*/
	//Q_PROPERTY(QColor highlightingColor READ highlightingColor WRITE setHighlightingColor);
	//QBrush _brushHighlighted;
	//QPen _penHighlighted;
	//QColor highlightingColor();
	//void setHighlightingColor(const QColor& color);

	/*!
		Property overwrite mode sets (setOverwriteMode()) or gets (overwriteMode()) the mode
		in which the editor works. In overwrite mode the user will overwrite existing data. The
		size of data will be constant. In insert mode the size will grow, when inserting
		new data.
	*/
	Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode);
	bool _overwriteMode;
	bool overwriteMode();
	void setOverwriteMode(bool overwriteMode);

	/*!
		Property selection color sets (setSelectionColor()) the background
		color of selected text areas. You can also read the color
		(selectionColor()).
	*/
	//Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor);
	//QBrush _brushSelection;
	//QPen _penSelection;
	//QColor selectionColor();
	//void setSelectionColor(const QColor& color);

	/*!
		Property readOnly sets (setReadOnly()) or gets (isReadOnly) the mode
		in which the editor works. In readonly mode the the user can only navigate
		through the data and select data; modifying is not possible. This
		property's default is false.
	*/
	Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly);
	bool _readOnly;
	bool isReadOnly();
	void setReadOnly(bool readOnly);

	/*! Set the font of the widget. Please use fixed width fonts like Mono or Courier.*/
	Q_PROPERTY(QFont font READ font WRITE setFont);
	void setFont(const QFont& font);

public:
	QHexEdit2(QWidget* parent);
	~QHexEdit2();

	bool setData(QIODevice& iODevice);
	void setBreak(int pos);

signals:
	void onBreakpointClick(int addr);
	void onBreakpointBreak(int addr);

protected:
	// Handle events
	void keyPressEvent(QKeyEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent* event);
	void resizeEvent(QResizeEvent*);
	virtual bool focusNextPrevChild(bool next);

private slots:
	void onVerticalScrollBarUpdate();
	void onHorizontalScrollBarUpdate();

private:
	// Launched when an event happens
	void onResizeReposition();
	void onSourceChanged();

	// Other functions
	void updateShownBuffer();
	void recalculateMetricVariables();
	void recalculateOverallWidth();
	QString formatAddress(int address);

private:
	/*
	 * Size and position variables. More or less known at compile time.
	 */
	/*! Width and height of our font characters. Read-only. */
	QSize _size_Font;

	/*! Overall width of the widget */
	int _size_Width_Overall;

	/*! Height of a single line. Unified value. Multiply by number of rows to get overall height */
	int _size_Height_SingleLine;

	/*! Width of an entire address area. Read-only. */
	int _size_Width_AddressAreaBox;

	/*! Width of an entire hex area. Read-only. */
	int _size_Width_HexAreaBox;

	/*! Right margin of hex area. */
	int _margin_right_HexAreaBox;

	/*! Width of an entire ascii area. Read-only. */
	int _size_Width_AsciiAreaBox;

	/*! Left margin of address area address field. Usually it's 2x font width. Read-only. */
	//in _margin_AddressAreaField;
	int _margin_left_AddressAreaField;
	int _margin_right_AddressAreaField;

	/*! Starting X position of hex area row. */
	//int _posX_start_HexAreaRow;

	/*! Starting X position of ascii area row */
	//int _posX_start_AsciiAreaField;

	/*! Width of a single breakpoint field. Read-only. Calculated from height of a line. */
	int _size_Width_BreakpointField;

	/*! Width of a single hex area field */
	int _size_Width_HexAreaField;

	/*! Width of a single ascii letter */
	int _size_Width_AsciiAreaField;

	/*
	 * Painting helper variables
	 */
	//int _rowsCanShowOnScreen; // Rows that can be shown on screen. Viewport height / single line height
	//int _rowsShown; // Rows that have to be shown on screen.

	/*
	 * Other variables
	 */
	Chunks* _chunks;
	//int _byteFirstShown;
	//int _byteLastShown;
	int _firstLine;
	int _allLines;
	int _linesToShow;
	QByteArray _dataShownOnScreen; // data in the current View
	QByteArray _hexDataShown;
	QBuffer _buffer;
	std::vector<BreakpointType> _breakpoints;
};
