#include "file_regitration.h"
#include <windows.h>

//void FileRegistration::registerFileType(const QString& documentId,
//                                      const QString& fileTypeName,
//                                      const QString& fileExtension,
//                                      qint32         appIconIndex)
//{
//    // first register the type ID of our server
//    if (!SetHkcrUserRegKey(documentId, fileTypeName))
//        return;

//    if (!SetHkcrUserRegKey(QString("%1\\DefaultIcon").arg(documentId),
//                   QString("\"%1\",%2").arg(QDir::toNativeSeparators(qApp->applicationFilePath())).arg(appIconIndex)))
//        return;

//    LONG lSize = _MAX_PATH * 2;
//    wchar_t szTempBuffer[_MAX_PATH * 2];
//    LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT,
//                                   (const wchar_t*)fileExtension.utf16(),
//                                   szTempBuffer,
//                                   &lSize);

//    QString temp = QString::fromUtf16((unsigned short*)szTempBuffer);

//    if (lResult != ERROR_SUCCESS || temp.isEmpty() || temp == documentId)
//    {
//        // no association for that suffix
//        if (!SetHkcrUserRegKey(fileExtension, documentId))
//            return;

//        SetHkcrUserRegKey(QString("%1\\ShellNew").arg(fileExtension), QLatin1String(""), QLatin1String("NullFile"));
//    }
//}

//bool FileRegistration::SetHkcrUserRegKey(QString key, const QString& value, const QString& valueName)
//{
//    HKEY hKey;

//    key.prepend("Software\\Classes\\");

//    LONG lRetVal = RegCreateKey(HKEY_CURRENT_USER,
//                                (const wchar_t*)key.utf16(),
//                                &hKey);

//    if(ERROR_SUCCESS == lRetVal)
//    {
//        LONG lResult = ::RegSetValueExW(hKey,
//                                       valueName.isEmpty() ? 0 : (const wchar_t*)valueName.utf16(),
//                                       0,
//                                       REG_SZ,
//                                       (CONST BYTE*)value.utf16(),
//                                       (value.length() + 1) * sizeof(wchar_t));

//        if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
//            return true;

//        QMessageBox::warning(0, QString("Error in setting Registry values"),
//                             QString("registration database update failed for key '%s'.").arg(key));
//    }
//    else
//    {
//        wchar_t buffer[4096];
//        ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 0, lRetVal, 0, buffer, 4096, 0);
//        QString szText = QString::fromUtf16((const ushort*)buffer);
//        QMessageBox::warning(this, QString("Error in setting Registry values"), szText);
//    }
//    return false;
//}
