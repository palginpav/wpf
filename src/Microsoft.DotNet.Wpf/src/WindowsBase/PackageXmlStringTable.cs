//-------------------------------------------------------------------------------
// <copyright from='1999' to='2005' company='Microsoft Corporation'>
//    Copyright (c) Microsoft Corporation. All Rights Reserved.
//    Information Contained Herein is Proprietary and Confidential.
// </copyright>
//
// This file is generated from MS/Internal/IO/Packaging/PackageXmlNamespaces.txt and MS/Internal/IO/Packaging/PackageXmlStrings.txt by GenXmlStringTable.pl
//           - do not modify this file directly
//-------------------------------------------------------------------------------


using System;
using System.Collections;
using System.Diagnostics;
using System.Xml;

using MS.Internal.IO.Packaging;

namespace MS.Internal.IO.Packaging
{

    //an enums for xml string identifiers.
    internal enum PackageXmlEnum : int
    {
        NotDefined = 0,
        XmlSchemaInstanceNamespace,
        XmlSchemaInstanceNamespacePrefix,
        XmlNamespacePrefix,
        PackageCorePropertiesNamespace,
        DublinCorePropertiesNamespace,
        DublinCoreTermsNamespace,
        DublinCorePropertiesNamespacePrefix,
        DublincCoreTermsNamespacePrefix,
        CoreProperties,
        Type,
        Creator,
        Identifier,
        Title,
        Subject,
        Description,
        Language,
        Created,
        Modified,
        ContentType,
        Keywords,
        Category,
        Version,
        LastModifiedBy,
        ContentStatus,
        Revision,
        LastPrinted,
    }   // end of enum PackageXmlEnum
}   // end of namespace

namespace MS.Internal.IO.Packaging
{
    internal static class PackageXmlStringTable
    {
        static PackageXmlStringTable()
        {
            Object str;

             str = _nameTable.Add("http://www.w3.org/2001/XMLSchema-instance");
             _xmlstringtable[(int) PackageXmlEnum.XmlSchemaInstanceNamespace] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, null);
             str = _nameTable.Add("xsi");
             _xmlstringtable[(int) PackageXmlEnum.XmlSchemaInstanceNamespacePrefix] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, null);
             str = _nameTable.Add("xmlns");
             _xmlstringtable[(int) PackageXmlEnum.XmlNamespacePrefix] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, null);
             str = _nameTable.Add("http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
             _xmlstringtable[(int) PackageXmlEnum.PackageCorePropertiesNamespace] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, null);
             str = _nameTable.Add("http://purl.org/dc/elements/1.1/");
             _xmlstringtable[(int) PackageXmlEnum.DublinCorePropertiesNamespace] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, null);
             str = _nameTable.Add("http://purl.org/dc/terms/");
             _xmlstringtable[(int) PackageXmlEnum.DublinCoreTermsNamespace] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, null);
             str = _nameTable.Add("dc");
             _xmlstringtable[(int) PackageXmlEnum.DublinCorePropertiesNamespacePrefix] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, null);
             str = _nameTable.Add("dcterms");
             _xmlstringtable[(int) PackageXmlEnum.DublincCoreTermsNamespacePrefix] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, null);

             str = _nameTable.Add("coreProperties");
             _xmlstringtable[(int) PackageXmlEnum.CoreProperties] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "NotSpecified");
             str = _nameTable.Add("type");
             _xmlstringtable[(int) PackageXmlEnum.Type] = new XmlStringTableStruct(str, PackageXmlEnum.NotDefined, "NotSpecified");
             str = _nameTable.Add("creator");
             _xmlstringtable[(int) PackageXmlEnum.Creator] = new XmlStringTableStruct(str, PackageXmlEnum.DublinCorePropertiesNamespace, "String");
             str = _nameTable.Add("identifier");
             _xmlstringtable[(int) PackageXmlEnum.Identifier] = new XmlStringTableStruct(str, PackageXmlEnum.DublinCorePropertiesNamespace, "String");
             str = _nameTable.Add("title");
             _xmlstringtable[(int) PackageXmlEnum.Title] = new XmlStringTableStruct(str, PackageXmlEnum.DublinCorePropertiesNamespace, "String");
             str = _nameTable.Add("subject");
             _xmlstringtable[(int) PackageXmlEnum.Subject] = new XmlStringTableStruct(str, PackageXmlEnum.DublinCorePropertiesNamespace, "String");
             str = _nameTable.Add("description");
             _xmlstringtable[(int) PackageXmlEnum.Description] = new XmlStringTableStruct(str, PackageXmlEnum.DublinCorePropertiesNamespace, "String");
             str = _nameTable.Add("language");
             _xmlstringtable[(int) PackageXmlEnum.Language] = new XmlStringTableStruct(str, PackageXmlEnum.DublinCorePropertiesNamespace, "String");
             str = _nameTable.Add("created");
             _xmlstringtable[(int) PackageXmlEnum.Created] = new XmlStringTableStruct(str, PackageXmlEnum.DublinCoreTermsNamespace, "DateTime");
             str = _nameTable.Add("modified");
             _xmlstringtable[(int) PackageXmlEnum.Modified] = new XmlStringTableStruct(str, PackageXmlEnum.DublinCoreTermsNamespace, "DateTime");
             str = _nameTable.Add("contentType");
             _xmlstringtable[(int) PackageXmlEnum.ContentType] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "String");
             str = _nameTable.Add("keywords");
             _xmlstringtable[(int) PackageXmlEnum.Keywords] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "String");
             str = _nameTable.Add("category");
             _xmlstringtable[(int) PackageXmlEnum.Category] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "String");
             str = _nameTable.Add("version");
             _xmlstringtable[(int) PackageXmlEnum.Version] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "String");
             str = _nameTable.Add("lastModifiedBy");
             _xmlstringtable[(int) PackageXmlEnum.LastModifiedBy] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "String");
             str = _nameTable.Add("contentStatus");
             _xmlstringtable[(int) PackageXmlEnum.ContentStatus] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "String");
             str = _nameTable.Add("revision");
             _xmlstringtable[(int) PackageXmlEnum.Revision] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "String");
             str = _nameTable.Add("lastPrinted");
             _xmlstringtable[(int) PackageXmlEnum.LastPrinted] = new XmlStringTableStruct(str, PackageXmlEnum.PackageCorePropertiesNamespace, "DateTime");

        }

        internal static PackageXmlEnum GetEnumOf(Object xmlString)
        {
            Debug.Assert(xmlString is String);

			// Index 0 is reserved for NotDefined and doesn't have a table entry
			//	so start from 1
            for (int i = 1; i < _xmlstringtable.GetLength(0) ; ++i)
            {
                if (Object.ReferenceEquals(_xmlstringtable[i].Name, xmlString))
                {
                    return ((PackageXmlEnum) i);
                }
            }

            return PackageXmlEnum.NotDefined;
        }

        internal static string GetXmlString(PackageXmlEnum id)
        {
			CheckIdRange(id);

            return (string) _xmlstringtable[(int) id].Name;
        }

        internal static Object GetXmlStringAsObject(PackageXmlEnum id)
        {
			CheckIdRange(id);

            return _xmlstringtable[(int) id].Name;
        }

        internal static PackageXmlEnum GetXmlNamespace(PackageXmlEnum id)
        {
			CheckIdRange(id);

            return _xmlstringtable[(int) id].Namespace;
        }

        internal static string GetValueType(PackageXmlEnum id)
        {
			CheckIdRange(id);

            return _xmlstringtable[(int) id].ValueType;
        }

        internal static NameTable NameTable
        {
            get
            {
                return _nameTable;
            }
        }

#if false
        internal static IEqualityComparer EqualityComparer
        {
            get
            {
                return _referenceComparer;
            }
        }
#endif

		private static void CheckIdRange(PackageXmlEnum id)
		{
			// Index 0 is reserved for NotDefined and doesn't have a table entry
        	if ((int) id <= 0 || (int) id >= 27)
        	{
        		throw new ArgumentOutOfRangeException("id");
        	}
        }

        internal static NameTable CloneNameTable()
        {
            NameTable nameTable = new NameTable();

            // Index 0 is reserved for NotDefined and doesn't have a table entry
            for (int i=1; i<27; ++i)
            {
                nameTable.Add((string)_xmlstringtable[i].Name);
            }

            return nameTable;
        }

        private struct XmlStringTableStruct
        {
            private Object _nameString;
            private PackageXmlEnum _namespace;
            private string _valueType;

            internal XmlStringTableStruct(Object nameString, PackageXmlEnum ns, string valueType)
            {
                _nameString = nameString;
                _namespace = ns;
                _valueType = valueType;
            }

            internal Object Name { get { return (String) _nameString; } }
            internal PackageXmlEnum Namespace { get { return _namespace; } }
            internal string ValueType { get { return _valueType; } }
        }

#if false
        // The Hashtable comparer that takes advantage of the fact
        // that we know the object identities of all keys to find.
        private class ReferenceComparer : IEqualityComparer
        {
            // Perform reference comparison.
            // Explicit implementation to avoid conflict with object.Equals.
            bool IEqualityComparer.Equals(object x, object y)
            {
                return object.ReferenceEquals(x, y);
            }

            // Hash on object identity.
            public int GetHashCode(object obj)
            {
                return System.Runtime.CompilerServices.RuntimeHelpers.GetHashCode(obj);
            }
        }

#endif

        private static XmlStringTableStruct[] _xmlstringtable = new XmlStringTableStruct[27];
        private static NameTable _nameTable = new NameTable();
#if false
        private static ReferenceComparer _referenceComparer = new ReferenceComparer();
#endif
    }    //endof class PackageXmlStringTable

}   // end of namespace

