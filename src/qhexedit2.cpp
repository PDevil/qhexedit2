#include "qhexedit2.h"

#include <qpainter.h>
#include <qscrollbar.h>
#include <qrect.h>
#include <qevent.h>
#include <qpixmap.h>

QHexEdit2::QHexEdit2(QWidget* parent) : QAbstractScrollArea(parent)
{
	_addressArea = true;
	_addressAreaColor = QColor(46, 46, 46);
	_addressAreaFontColor = QColor(192, 192, 192);
	_hexArea = true;
	_addressOffset = 0;
	_addressWidth = 4;
	_asciiArea = true;
	_asciiAreaBackgroundColor = QColor(46, 46, 46);
	_bytesPerLine = 16;
	_cursorPosition = 0;
	_hexCaps = false;
	_overwriteMode = true;
	_readOnly = false;

	_chunks = new Chunks(this);
	_firstLine = 0;
	_allLines = 0;
	_linesToShow = 0;

	setFont(QFont("Consolas", 10));

	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVerticalScrollBarUpdate()));
	connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onHorizontalScrollBarUpdate()));
}

QHexEdit2::~QHexEdit2()
{
}

bool QHexEdit2::setData(QIODevice& iODevice)
{
	bool ok = _chunks->setIODevice(iODevice);
	this->onSourceChanged();
	return ok;
}

/*
 * Properties
 */

/* Address Area */
bool QHexEdit2::addressArea()
{
	return _addressArea;
}

void QHexEdit2::setAddressArea(bool addressArea)
{
	_addressArea = addressArea;
	recalculateOverallWidth();
	viewport()->update();
}

bool QHexEdit2::hexArea()
{
	return _hexArea;
}

void QHexEdit2::setHexArea(bool hexArea)
{
	_hexArea = hexArea;
	recalculateOverallWidth();
	viewport()->update();
}

/* Address Area Color */
QColor QHexEdit2::addressAreaColor()
{
	return _addressAreaColor;
}

void QHexEdit2::setAddressAreaColor(const QColor& color)
{
	_addressAreaColor = color;
	viewport()->update();
}

QColor QHexEdit2::addressAreaFontColor()
{
	return _addressAreaFontColor;
}

void QHexEdit2::setAddressAreaFontColor(const QColor& color)
{
	_addressAreaFontColor = color;
	viewport()->update();
}

qint64 QHexEdit2::addressOffset()
{
	return _addressOffset;
}

void QHexEdit2::setAddressOffset(qint64 addressOffset)
{
	_addressOffset = addressOffset;
	viewport()->update();
}

int QHexEdit2::addressWidth()
{
	/*qint64 size = _chunks->size();
	int n = 1;
	if (size > Q_INT64_C(0x100000000)) { n += 8; size /= Q_INT64_C(0x100000000); }
	if (size > 0x10000) { n += 4; size /= 0x10000; }
	if (size > 0x100) { n += 2; size /= 0x100; }
	if (size > 0x10) { n += 1; }

	if (n > _addressWidth)
		return n;
	else
		return _addressWidth;*/
	return _addressWidth;
}

void QHexEdit2::setAddressWidth(int addressWidth)
{
	_addressWidth = addressWidth;
	this->recalculateMetricVariables();
	this->setCursorPosition(_cursorPosition);
	viewport()->update();
}

bool QHexEdit2::asciiArea()
{
	return _asciiArea;
}

void QHexEdit2::setAsciiArea(bool asciiArea)
{
	_asciiArea = asciiArea;
	this->recalculateOverallWidth();
	this->setCursorPosition(_cursorPosition);
	viewport()->update();
}

QColor QHexEdit2::asciiAreaBackgroundColor()
{
	return _asciiAreaBackgroundColor;
}

void QHexEdit2::setAsciiAreaBackgroundColor(QColor color)
{
	color.setAlpha(255);
	_asciiAreaBackgroundColor = color;
}

int QHexEdit2::bytesPerLine()
{
	return _bytesPerLine;
}

void QHexEdit2::setBytesPerLine(int count)
{
	_bytesPerLine = count;
	this->recalculateMetricVariables();
	this->onResizeReposition();
	setCursorPosition(_cursorPosition);
	viewport()->update();
}

qint64 QHexEdit2::cursorPosition()
{
	return _cursorPosition;
}

void QHexEdit2::setCursorPosition(qint64 position)
{
	_cursorPosition = position;
	// 1. delete old cursor
	/*_blink = false;
	viewport()->update(_cursorRect);

	// 2. Check, if cursor in range?
	if (position > (_chunks->size() * 2 - 1))
		position = _chunks->size() * 2 - (_overwriteMode ? 1 : 0);

	if (position < 0)
		position = 0;

	// 3. Calc new position of cursor
	_bPosCurrent = position / 2;
	_pxCursorY = ((position / 2 - _bPosFirst) / _bytesPerLine + 1) * _pxCharHeight;
	int x = (position % (2 * _bytesPerLine));
	if (_editAreaIsAscii)
	{
		_pxCursorX = x / 2 * _pxCharWidth + _pxPosAsciiX;
		_cursorPosition = position & 0xFFFFFFFFFFFFFFFE;
	}
	else {
		_pxCursorX = (((x / 2) * 3) + (x % 2)) * _pxCharWidth + _pxPosAddrWidth;
		_cursorPosition = position;
	}

	if (_overwriteMode)
		_cursorRect = QRect(_pxCursorX - horizontalScrollBar()->value(), _pxCursorY + _pxCursorWidth, _pxCharWidth, _pxCursorWidth);
	else
		_cursorRect = QRect(_pxCursorX - horizontalScrollBar()->value(), _pxCursorY - _pxCharHeight + 4, _pxCursorWidth, _pxCharHeight);

	// 4. Immediately draw new cursor
	_blink = true;
	viewport()->update(_cursorRect);*/
}

QByteArray QHexEdit2::data()
{
	return _chunks->data(0, -1);
}

void QHexEdit2::setData(const QByteArray& ba)
{
	_buffer.setData(ba);
	setData(_buffer);
}

bool QHexEdit2::overwriteMode()
{
	return _overwriteMode;
}

void QHexEdit2::setOverwriteMode(bool overwriteMode)
{
	_overwriteMode = overwriteMode;
}

bool QHexEdit2::isReadOnly()
{
	return _readOnly;
}

void QHexEdit2::setReadOnly(bool readOnly)
{
	_readOnly = readOnly;
}

bool QHexEdit2::hexCaps()
{
	return _hexCaps;
}

void QHexEdit2::setHexCaps(const bool isCaps)
{
	if (_hexCaps != isCaps)
	{
		_hexCaps = isCaps;
		viewport()->update();
	}
}

bool QHexEdit2::dynamicBytesPerLine()
{
	return _dynamicBytesPerLine;
}

QColor QHexEdit2::breakpointFieldColor()
{
	return _breakpointFieldColor;
}

void QHexEdit2::setBreakpointFieldColor(QColor color)
{
	_breakpointFieldColor = color;
	viewport()->update();
}

QColor QHexEdit2::hexFieldColor()
{
	return _hexFieldColor;
}

void QHexEdit2::setHexFieldColor(QColor color)
{
	_hexFieldColor = color;
	viewport()->update();
}

void QHexEdit2::setDynamicBytesPerLine(const bool isDynamic)
{
	_dynamicBytesPerLine = isDynamic;
	resizeEvent(NULL);
}

void QHexEdit2::setFont(const QFont& font)
{
	QWidget::setFont(font);

	recalculateMetricVariables();
	recalculateOverallWidth();
	onResizeReposition();
	viewport()->update();
}

/*
 * Event handling
 */
void QHexEdit2::keyPressEvent(QKeyEvent* event)
{
}

void QHexEdit2::mouseMoveEvent(QMouseEvent* event)
{
}

void QHexEdit2::mousePressEvent(QMouseEvent* e)
{
	QPoint pos = e->pos();
	pos += QPoint(horizontalScrollBar()->value(), 0);
	if (hexArea() && !addressArea() || (addressArea() && pos.x() > _size_Width_AddressAreaBox))
	{
		if (addressArea())
		{
			pos -= QPoint(_size_Width_AddressAreaBox, 0);
		}

		int singleByteWidth = _size_Width_BreakpointField + _size_Width_HexAreaField;
		int byteXPos = pos.x() / singleByteWidth;
		int byteIndex =
			(_firstLine * this->bytesPerLine()) + // starting line
			(pos.y() / _size_Height_SingleLine * this->bytesPerLine()) + // lines above cursor
			(byteXPos); // current line

		if (byteIndex < _chunks->size() && pos.x() % singleByteWidth < _size_Width_BreakpointField)
		{
			emit onBreakpointClick(byteIndex);
			/*if (_breakpoints[byteIndex] == BreakpointType::None)
			{
				_breakpoints[byteIndex] = BreakpointType::Read;
			}
			else if (_breakpoints[byteIndex] == BreakpointType::Read)
			{
				_breakpoints[byteIndex] = BreakpointType::Write;
			}
			else if (_breakpoints[byteIndex] == BreakpointType::Write)
			{
				_breakpoints[byteIndex] = BreakpointType::ReadWrite;
			}
			else
			{
				_breakpoints[byteIndex] = BreakpointType::None;
			}*/
			viewport()->update();
		}
	}
}

void QHexEdit2::paintEvent(QPaintEvent* event)
{
	QPainter painter(viewport());
	auto Xoffset = -horizontalScrollBar()->value();
	auto Xcurrent = 0;

	painter.fillRect(0, 0, viewport()->width(), viewport()->height(), asciiAreaBackgroundColor());

	if (addressArea())
	{
		QRect addressAreaBox;
		auto x = Xcurrent + Xoffset;

		addressAreaBox.setRect(0, 0, _size_Width_AddressAreaBox, viewport()->height());
		painter.fillRect(addressAreaBox, addressAreaColor());

		painter.setPen(addressAreaFontColor());
		addressAreaBox.setRect(x + _margin_left_AddressAreaField, 0, _size_Width_AddressAreaBox, _size_Height_SingleLine);
		for (int row = 0, pxPosY = 0; row < this->_linesToShow; ++row, pxPosY += _size_Height_SingleLine)
		{
			addressAreaBox.moveTop(pxPosY);
			painter.drawText(addressAreaBox, Qt::AlignLeft | Qt::AlignVCenter, formatAddress((_firstLine + row) * bytesPerLine() + addressOffset()));
		}

		Xcurrent += _size_Width_AddressAreaBox;
	}

	if (hexArea())
	{
		auto x = Xcurrent + Xoffset;

		QRect box;
		box.setRect(0, 0, _size_Width_HexAreaField, _size_Height_SingleLine);
		painter.setPen(Qt::white);
		int relBytePos = 0;
		int absBytePos = _firstLine * this->bytesPerLine();
		for (int row = 0, pxPosY = 0; row < this->_linesToShow; ++row, pxPosY += _size_Height_SingleLine)
		{
			box.moveTop(pxPosY);
			for (int col = 0, x_pos = x;
				col < bytesPerLine() && relBytePos < _dataShownOnScreen.size();
				++col, ++relBytePos, ++absBytePos)
			{
				// Draw breakpoint box on the left of byte
				painter.fillRect(x_pos, pxPosY, _size_Width_BreakpointField, _size_Height_SingleLine, breakpointFieldColor());

				BreakpointType brk = _breakpoints[absBytePos];
				if (brk != BreakpointType::None)
				{
					QPixmap icon;
					if (brk == BreakpointType::Read)
					{
						icon = QPixmap(":/BrkR");
					}
					else if (brk == BreakpointType::Write)
					{
						icon = QPixmap(":/BrkW");
					}
					else
					{
						icon = QPixmap(":/BrkRW");
					}
					painter.drawPixmap(x_pos + 1, pxPosY + 1, _size_Width_BreakpointField - 2, _size_Height_SingleLine - 2, icon);
				}

				x_pos += _size_Width_BreakpointField;
				box.moveLeft(x_pos);
				
				// Draw hex box and text
				painter.fillRect(x_pos, pxPosY, _size_Width_HexAreaField, _size_Height_SingleLine, hexFieldColor());
				painter.drawText(box, Qt::AlignLeft | Qt::AlignVCenter, _hexDataShown.mid(relBytePos * 2, 2));
				x_pos += _size_Width_HexAreaField;
			}
		}

		painter.setPen(Qt::gray);
		int lineXpos = x + _size_Width_HexAreaBox - (_margin_right_HexAreaBox / 2);
		painter.drawLine(lineXpos, 0, lineXpos, viewport()->height());

		Xcurrent += _size_Width_HexAreaBox;
	}

	if (asciiArea())
	{
		auto x = Xcurrent + Xoffset;

		QRect box;
		box.setRect(x, 0, _size_Width_AsciiAreaField, _size_Height_SingleLine);
		painter.setPen(Qt::white);
		int bytePos = 0;
		for(int row = 0, pxPosY = 0;
			row < this->_linesToShow;
			++row, pxPosY += _size_Height_SingleLine)
		{
			box.moveTop(pxPosY);

			for (int col = 0, x_pos = x;
				col < bytesPerLine() && bytePos < _dataShownOnScreen.size();
				++col, ++bytePos)
			{
				box.moveLeft(x_pos);

				QChar ch = (uchar) _dataShownOnScreen.at(bytePos);
				if (ch < ' ' || ch > 0x7F)
					ch = '.';

				painter.drawText(box, Qt::AlignLeft | Qt::AlignVCenter, ch);
				x_pos += _size_Width_AsciiAreaField;
			}
		}
	}
}

void QHexEdit2::resizeEvent(QResizeEvent* event)
{
	onResizeReposition();
}

bool QHexEdit2::focusNextPrevChild(bool next)
{
	return false;
}

/*
 * Scrollbars
 */
void QHexEdit2::onVerticalScrollBarUpdate()
{
	this->onResizeReposition();
}

void QHexEdit2::onHorizontalScrollBarUpdate()
{

}

void QHexEdit2::onResizeReposition()
{
	int vW = viewport()->width();
	int vH = viewport()->height();
	int oldFirstLine = _firstLine;
	int oldLinesToShow = _linesToShow;
	_firstLine = verticalScrollBar()->value();
	_linesToShow = vH / _size_Height_SingleLine + (vH % _size_Height_SingleLine > 0);
	int linesLeft = _allLines - _firstLine;
	if (_linesToShow > linesLeft)
	{
		_linesToShow = linesLeft;
	}

	horizontalScrollBar()->setRange(0, _size_Width_Overall - vW);
	horizontalScrollBar()->setPageStep(vW);

	verticalScrollBar()->setRange(0, _allLines - (vH / _size_Height_SingleLine));
	verticalScrollBar()->setPageStep(_linesToShow);
	if (oldFirstLine != _firstLine || oldLinesToShow != _linesToShow)
	{
		int rowsOverall = _chunks->size() / bytesPerLine() + 1;

		this->updateShownBuffer();
	}
}

void QHexEdit2::onSourceChanged()
{
	_allLines = _chunks->size() / this->bytesPerLine() + (_chunks->size() % this->bytesPerLine() > 0);
	_breakpoints.resize(_chunks->size(), BreakpointType::None);
	this->onResizeReposition();
	viewport()->update();
}

void QHexEdit2::updateShownBuffer()
{
	_dataShownOnScreen = _chunks->data(_firstLine * this->bytesPerLine(), _linesToShow * this->bytesPerLine(), nullptr);
	_hexDataShown = _dataShownOnScreen.toHex();
}

void QHexEdit2::recalculateMetricVariables()
{
	QFontMetrics metrics(font());

	// Global
	_size_Font = QSize(metrics.averageCharWidth(), metrics.lineSpacing());
	_size_Height_SingleLine = _size_Font.height() * 1.3; // With font height changes

	// Address area
	int addressFieldSize = _size_Font.width() * (addressWidth() + ((addressWidth() - 1) / 2));
	_margin_left_AddressAreaField = _size_Font.width() * 2;
	_margin_right_AddressAreaField = _size_Font.width() / 2;
	_size_Width_AddressAreaBox = _margin_left_AddressAreaField + addressFieldSize + _margin_right_AddressAreaField;

	// Hex area
	_margin_right_HexAreaBox = _size_Font.width() * 2;
	_size_Width_BreakpointField = _size_Height_SingleLine; // With height change this changes too
	_size_Width_HexAreaField = _size_Font.width() * 2;
	_size_Width_HexAreaBox = bytesPerLine() * (_size_Width_BreakpointField + _size_Width_HexAreaField) + _margin_right_HexAreaBox;

	// Ascii area
	_size_Width_AsciiAreaField = _size_Font.width();
	_size_Width_AsciiAreaBox = bytesPerLine() * _size_Width_AsciiAreaField;

	this->recalculateOverallWidth();
}

void QHexEdit2::recalculateOverallWidth()
{
	_size_Width_Overall = 0;
	if (addressArea())
		_size_Width_Overall += _size_Width_AddressAreaBox;

	if (hexArea())
		_size_Width_Overall += _size_Width_HexAreaBox;

	if (asciiArea())
		_size_Width_Overall += _size_Width_AsciiAreaBox;
}

QString QHexEdit2::formatAddress(int address)
{
	QString addr = QString("%1").arg(address, addressWidth(), 16, QChar('0'));
	if (hexCaps())
		addr = addr.toUpper();

	for (int i = 2; i < addressWidth(); i += 2)
	{
		addr.insert(i, ' ');
	}
	return addr;
}
