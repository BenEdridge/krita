/* This file is part of the KDE project
 * Copyright (C) 2006-2007 Thomas Zander <zander@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "KoListLevelProperties.h"
#include "Styles_p.h"
//#include "KoTextBlockData.h"

//#include <QTextCursor>
//#include <QSharedData>
#include <kdebug.h>

#include <KoXmlNS.h>
#include <KoOdfLoadingContext.h>
#include <KoXmlWriter.h>

class KoListLevelProperties::Private {
public:
    StylePrivate stylesPrivate;
    //QMap<const QTextDocument*, QPointer<QTextList> > textLists;

    void copy(Private *other) {
        stylesPrivate = other->stylesPrivate;
        //textLists = other->textLists;
    }
};

// all relevant properties.
static const int properties[] = {
    QTextListFormat::ListStyle,
    KoListStyle::ListItemPrefix,
    KoListStyle::ListItemSuffix,
    KoListStyle::StartValue,
    KoListStyle::Level,
    KoListStyle::DisplayLevel,
    KoListStyle::CharacterStyleId,
    KoListStyle::BulletCharacter,
    KoListStyle::BulletSize,
    KoListStyle::Alignment,
    KoListStyle::LetterSynchronization,
    KoListStyle::StyleId,
    -1
};

KoListLevelProperties::KoListLevelProperties()
    : d( new Private())
{
    setStyle(KoListStyle::DecimalItem);
    setListItemSuffix(".");
    setStartValue(1);
}

KoListLevelProperties::KoListLevelProperties(const KoListLevelProperties &other)
    : d( new Private())
{
    d->copy(other.d);
}

KoListLevelProperties::~KoListLevelProperties() {
    delete d;
}

int KoListLevelProperties::styleId() const 
{
    return propertyInt(KoListStyle::StyleId);
}

void KoListLevelProperties::setStyleId(int id) 
{
    setProperty(KoListStyle::StyleId, id);
}

void KoListLevelProperties::setProperty(int key, const QVariant &value) {
    d->stylesPrivate.add(key, value);
}

int KoListLevelProperties::propertyInt(int key) const {
    QVariant variant = d->stylesPrivate.value(key);
    if(variant.isNull())
        return 0;
    return variant.toInt();
}

double KoListLevelProperties::propertyDouble(int key) const {
    QVariant variant = d->stylesPrivate.value(key);
    if(variant.isNull())
        return 0.;
    return variant.toDouble();
}

bool KoListLevelProperties::propertyBoolean(int key) const {
    QVariant variant = d->stylesPrivate.value(key);
    if(variant.isNull())
        return false;
    return variant.toBool();
}

QString KoListLevelProperties::propertyString(int key) const {
    QVariant variant = d->stylesPrivate.value(key);
    if(variant.isNull())
        return QString();
    return qvariant_cast<QString>(variant);
}

void KoListLevelProperties::applyStyle(QTextListFormat &format) const {
    int i=0;
    while(properties[i] != -1) {
        QVariant variant = d->stylesPrivate.value(properties[i]);
        if(! variant.isNull())
            format.setProperty(properties[i], variant);
        i++;
    }
}

bool KoListLevelProperties::operator==(const KoListLevelProperties &other) const {
    // TODO move this to the stylesPrivate

    int i=0;
    while(properties[i] != -1) {
        QVariant variant = d->stylesPrivate.value(properties[i]);
        if(! variant.isNull()) {
            if(other.d->stylesPrivate.value(properties[i]) != 0)
                return false;
        }
        else {
            QVariant otherVariant = d->stylesPrivate.value(properties[i]);
            if(otherVariant == 0 || otherVariant != variant)
                return false;
        }
        i++;
    }
    return true;
}

void KoListLevelProperties::setListItemPrefix(const QString &prefix) {
    setProperty(KoListStyle::ListItemPrefix, prefix );
}

QString KoListLevelProperties::listItemPrefix() const {
    return propertyString(KoListStyle::ListItemPrefix);
}

void KoListLevelProperties::setStyle(KoListStyle::Style style) {
    setProperty(QTextListFormat::ListStyle, (int) style);
}

KoListStyle::Style KoListLevelProperties::style() const {
    return static_cast<KoListStyle::Style> (propertyInt(QTextListFormat::ListStyle));
}

void KoListLevelProperties::setListItemSuffix(const QString &suffix) {
    setProperty(KoListStyle::ListItemSuffix, suffix  );
}

QString KoListLevelProperties::listItemSuffix() const {
    return propertyString(KoListStyle::ListItemSuffix);
}

void KoListLevelProperties::setStartValue(int value) {
    setProperty(KoListStyle::StartValue, value  );
}

int KoListLevelProperties::startValue() const {
    return propertyInt (KoListStyle::StartValue);
}

void KoListLevelProperties::setLevel(int value) {
    setProperty(KoListStyle::Level, value  );
}

int KoListLevelProperties::level() const {
    return propertyInt (KoListStyle::Level);
}

void KoListLevelProperties::setDisplayLevel(int level) {
    setProperty(KoListStyle::DisplayLevel, level  );
}

int KoListLevelProperties::displayLevel() const {
    return propertyInt (KoListStyle::DisplayLevel);
}

void KoListLevelProperties::setCharacterStyleId(int id) {
    setProperty(KoListStyle::CharacterStyleId, id  );
}

int KoListLevelProperties::characterStyleId() const {
    return propertyInt (KoListStyle::CharacterStyleId);
}

void KoListLevelProperties::setBulletCharacter(QChar character) {
    setProperty(KoListStyle::BulletCharacter, (int) character.unicode() );
}

QChar KoListLevelProperties::bulletCharacter() const {
    return propertyInt (KoListStyle::BulletCharacter);
}

void KoListLevelProperties::setRelativeBulletSize(int percent) {
    setProperty(KoListStyle::BulletSize, percent  );
}

int KoListLevelProperties::relativeBulletSize() const {
    return propertyInt (KoListStyle::BulletSize);
}

void KoListLevelProperties::setAlignment(Qt::Alignment align) {
    setProperty(KoListStyle::Alignment, static_cast<int> (align) );
}

Qt::Alignment KoListLevelProperties::alignment() const {
    return static_cast<Qt::Alignment>(propertyInt(KoListStyle::Alignment));
}

void KoListLevelProperties::setMinimumWidth(double width) {
    setProperty(KoListStyle::MinimumWidth, width);
}

double KoListLevelProperties::minimumWidth() {
    return propertyDouble(KoListStyle::MinimumWidth);
}

KoListLevelProperties & KoListLevelProperties::operator=(const KoListLevelProperties &other) {
    d->copy(other.d);
    return *this;
}

void KoListLevelProperties::setListId(const QString &listId) {
    setProperty(KoListStyle::ListId, listId);
}

QString KoListLevelProperties::listId() const {
    return propertyString(KoListStyle::ListId);
}

bool KoListLevelProperties::letterSynchronization() const {
    return propertyBoolean(KoListStyle::LetterSynchronization);
}

void KoListLevelProperties::setLetterSynchronization(bool on) {
    setProperty(KoListStyle::LetterSynchronization, on);
}


// static
KoListLevelProperties KoListLevelProperties::fromTextList(QTextList *list) {
    KoListLevelProperties llp;
    QTextListFormat format = list->format();
    int i=0;
    while(properties[i] != -1) {
        int key = properties[i];
        if(format.hasProperty(key))
            llp.setProperty(key, format.property(key));
        i++;
    }
    return llp;
}

void KoListLevelProperties::loadOdf(KoOdfLoadingContext& context, const KoXmlElement& style)
{
    Q_UNUSED( context );

    // The text:level attribute specifies the level of the number list
    // style. It can be used on all list-level styles.
    const int level = qMax(1, style.attributeNS( KoXmlNS::text, "level", QString() ).toInt() );
    // The text:display-levels attribute specifies the number of
    // levels whose numbers are displayed at the current level.
    const QString displayLevel = style.attributeNS( KoXmlNS::text,
                                                    "display-levels", QString() );

    if ( style.localName() == "list-level-style-bullet" ) { // list with bullets

        //1.6: KoParagCounter::loadOasisListStyle
        QString bulletChar = style.isNull() ? QString() : style.attributeNS( KoXmlNS::text, "bullet-char", QString() );
        kDebug(32500)<<"style.localName()="<<style.localName()<<"level="<<level<<"displayLevel="<<displayLevel<<"bulletChar="<<bulletChar;
        if( bulletChar.isEmpty() ) { // list without any visible bullets
            setStyle(KoListStyle::NoItem);
        }
        else { // try to determinate the bullet we should use
            switch( bulletChar[0].unicode() ) {
                case 0x2022: // bullet, a small disc -> circle
                    //TODO use BulletSize to differ between small and large discs
                    setStyle(KoListStyle::CircleItem);
                    break;
                case 0x25CF: // black circle, large disc -> disc
                case 0xF0B7: // #113361
                    setStyle(KoListStyle::DiscItem);
                    break;
                case 0xE00C: // losange => rhombus
                    setStyle(KoListStyle::RhombusItem);
                    break;
                case 0xE00A: // square. Not in OASIS (reserved Unicode area!), but used in both OOo and kotext.
                    setStyle(KoListStyle::SquareItem);
                    break;
                case 0x27A2: // two-colors right-pointing triangle
                    setStyle(KoListStyle::RightArrowHeadItem);
                    break;
                case 0x2794: // arrow to right
                    setStyle(KoListStyle::RightArrowItem);
                    break;
                case 0x2714: // checkmark
                    setStyle(KoListStyle::HeavyCheckMarkItem);
                    break;
                case 0x2d: // minus
                    setStyle(KoListStyle::CustomCharItem);
                    setBulletCharacter('-');
                    break;
                case 0x2717: // cross
                    setStyle(KoListStyle::BallotXItem);
                    break;
                default:
                    QChar customBulletChar = bulletChar[0];
                    kDebug(32500) <<"Unhandled bullet code 0x" << QString::number( (uint)customBulletChar.unicode(), 16 );
                    kDebug(32500) << "Should use the style =>" << style.attributeNS( KoXmlNS::text, "style-name", QString::null ) << "<=";
                    setStyle(KoListStyle::CustomCharItem);
                    setBulletCharacter(customBulletChar);
                    /*
                    QString customBulletFont;
                    // often StarSymbol when it comes from OO; doesn't matter, Qt finds it in another font if needed.
                    if ( listStyleProperties.hasAttributeNS( KoXmlNS::style, "font-name" ) )
                    {
                        customBulletFont = listStyleProperties.attributeNS( KoXmlNS::style, "font-name", QString::null );
                        kDebug(32500) <<"customBulletFont style:font-name =" << listStyleProperties.attributeNS( KoXmlNS::style,"font-name", QString::null );
                    }
                    else if ( listStyleTextProperties.hasAttributeNS( KoXmlNS::fo, "font-family" ) )
                    {
                        customBulletFont = listStyleTextProperties.attributeNS( KoXmlNS::fo, "font-family", QString::null );
                        kDebug(32500) <<"customBulletFont fo:font-family =" << listStyleTextProperties.attributeNS( KoXmlNS::fo,"font-family", QString::null );
                    }
                    // ## TODO in fact we're supposed to read it from the style pointed to by text:style-name
                    */
//                     setStyle(KoListStyle::BoxItem); //fallback
                    break;
            }
        }

    }
    else if( style.localName() == "list-level-style-number" || style.localName() == "outline-level-style" ) { // it's a numbered list
        const QString format = style.attributeNS( KoXmlNS::style, "num-format", QString() );
        kDebug(32500)<<"style.localName()="<<style.localName()<<"level="<<level<<"displayLevel="<<displayLevel<<"format="<<format;
        if( format.isEmpty() ) {
            setStyle(KoListStyle::NoItem);
        }
        else {
            if( format[0] == '1' )
                setStyle(KoListStyle::DecimalItem);
            else if( format[0] == 'a' )
                setStyle(KoListStyle::AlphaLowerItem);
            else if( format[0] == 'A' )
                setStyle(KoListStyle::UpperAlphaItem);
            else if( format[0] == 'i' )
                setStyle(KoListStyle::RomanLowerItem);
            else if( format[0] == 'I' )
                setStyle(KoListStyle::UpperRomanItem);
            else {
                kDebug(32500)<<"list-level-style-number fallback!";
                setStyle(KoListStyle::DecimalItem); // fallback
            }
        }

        //The style:num-prefix and style:num-suffix attributes specify what to display before and after the number.
        const QString prefix = style.attributeNS( KoXmlNS::style, "num-prefix", QString() );
        if( ! prefix.isNull() )
            setListItemPrefix(prefix);
        const QString suffix = style.attributeNS( KoXmlNS::style, "num-suffix", QString() );
        if( ! suffix.isNull() )
            setListItemSuffix(suffix);
        const QString startValue = style.attributeNS( KoXmlNS::text, "start-value", QString() );
        if ( ! startValue.isNull() )
            setStartValue( startValue.toInt() );
    }
    else { // if not defined, we have do nothing
        kDebug(32500)<<"stylename else:"<<style.localName()<<"level="<<level<<"displayLevel="<<displayLevel;
        setStyle(KoListStyle::DecimalItem);
        setListItemSuffix(".");
    }

    setLevel(level);
    if( ! displayLevel.isNull() )
        setDisplayLevel(displayLevel.toInt());
}

void KoListLevelProperties::saveOdf (KoXmlWriter *writer) const
{
    bool isNumber = false;
    switch (d->stylesPrivate.value(QTextListFormat::ListStyle).toInt()) {
	case KoListStyle::DecimalItem:
	case KoListStyle::AlphaLowerItem:
	case KoListStyle::UpperAlphaItem:
	case KoListStyle::RomanLowerItem:
	case KoListStyle::UpperRomanItem:
	    isNumber = true;
	    break;
    }
    if (isNumber)
	writer->startElement("list-level-style-number");
    else
	writer->startElement("text:list-style-content");
    
    if (d->stylesPrivate.contains(KoListStyle::Level))
	writer->addAttribute("text:level", d->stylesPrivate.value(KoListStyle::Level).toInt());
    if (isNumber)
    {
	if (d->stylesPrivate.contains(KoListStyle::StartValue))
	    writer->addAttribute("text:start-value", QChar(d->stylesPrivate.value(KoListStyle::StartValue).toInt()));
	if (d->stylesPrivate.contains(KoListStyle::DisplayLevel))
	    writer->addAttribute("text:display-level", QChar(d->stylesPrivate.value(KoListStyle::DisplayLevel).toInt()));
    } else {
        if (d->stylesPrivate.contains(KoListStyle::BulletCharacter))
	    writer->addAttribute("text:bullet-char", QChar(d->stylesPrivate.value(KoListStyle::BulletCharacter).toInt()));
	
    }
    kDebug() << "Key KoListStyle::ListItemPrefix :" << d->stylesPrivate.value(KoListStyle::ListItemPrefix);
    kDebug() << "Key KoListStyle::ListItemSuffix :" << d->stylesPrivate.value(KoListStyle::ListItemSuffix);
    kDebug() << "Key KoListStyle::CharacterStyleId :" << d->stylesPrivate.value(KoListStyle::CharacterStyleId);
    kDebug() << "Key KoListStyle::BulletSize :" << d->stylesPrivate.value(KoListStyle::BulletSize);
    kDebug() << "Key KoListStyle::Alignment :" << d->stylesPrivate.value(KoListStyle::Alignment);
    kDebug() << "Key KoListStyle::LetterSynchronization :" << d->stylesPrivate.value(KoListStyle::LetterSynchronization);
    writer->endElement();
}
