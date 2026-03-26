// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Copyright (c) 2007 Novell, Inc. (http://www.novell.com)
//
// Authors:
//	Chris Toshok (toshok@ximian.com)
//

using System;
using System.Security;

namespace System.Security.Permissions {

	public sealed class WebBrowserPermission : CodeAccessPermission, IUnrestrictedPermission
	{
		private WebBrowserPermissionLevel _webBrowserPermissionLevel;

		public WebBrowserPermission ()
		{
			_webBrowserPermissionLevel = WebBrowserPermissionLevel.Safe;
		}

		public WebBrowserPermission (PermissionState state)
		{
			if (state == PermissionState.Unrestricted)
				_webBrowserPermissionLevel = WebBrowserPermissionLevel.Unrestricted;
			else if (state == PermissionState.None)
				_webBrowserPermissionLevel = WebBrowserPermissionLevel.None;
			else
				throw new ArgumentException ("InvalidPermissionState");
		}

		public WebBrowserPermission (WebBrowserPermissionLevel webBrowserPermissionLevel)
		{
			VerifyWebBrowserPermissionLevel (webBrowserPermissionLevel);
			_webBrowserPermissionLevel = webBrowserPermissionLevel;
		}

		public WebBrowserPermissionLevel Level {
			get { return _webBrowserPermissionLevel; }
			set {
				VerifyWebBrowserPermissionLevel (value);
				_webBrowserPermissionLevel = value;
			}
		}

		public override IPermission Copy ()
		{
			return new WebBrowserPermission (_webBrowserPermissionLevel);
		}

		public override void FromXml (SecurityElement securityElement)
		{
			if (securityElement == null)
				throw new ArgumentNullException ("securityElement");

			string classAttr = securityElement.Attribute ("class");
			if (classAttr == null || classAttr.IndexOf (GetType ().FullName, StringComparison.Ordinal) == -1)
				throw new ArgumentNullException ("securityElement");

			string unrestricted = securityElement.Attribute ("Unrestricted");
			if (unrestricted != null && bool.TrueString.Equals (unrestricted, StringComparison.OrdinalIgnoreCase)) {
				_webBrowserPermissionLevel = WebBrowserPermissionLevel.Unrestricted;
				return;
			}

			string level = securityElement.Attribute ("Level");
			if (level != null)
				_webBrowserPermissionLevel = (WebBrowserPermissionLevel) Enum.Parse (typeof (WebBrowserPermissionLevel), level);
			else
				_webBrowserPermissionLevel = WebBrowserPermissionLevel.None;
		}

		public override IPermission Intersect (IPermission target)
		{
			if (target == null)
				return null;

			WebBrowserPermission other = target as WebBrowserPermission;
			if (other == null)
				throw new ArgumentException ("TargetNotWebBrowserPermissionLevel");

			WebBrowserPermissionLevel level = _webBrowserPermissionLevel < other._webBrowserPermissionLevel ? _webBrowserPermissionLevel : other._webBrowserPermissionLevel;
			return new WebBrowserPermission (level);
		}

		public override bool IsSubsetOf (IPermission target)
		{
			if (target == null)
				return _webBrowserPermissionLevel == WebBrowserPermissionLevel.None;

			WebBrowserPermission other = target as WebBrowserPermission;
			if (other == null)
				throw new ArgumentException ("TargetNotWebBrowserPermissionLevel");

			return _webBrowserPermissionLevel <= other._webBrowserPermissionLevel;
		}

		public bool IsUnrestricted ()
		{
			return _webBrowserPermissionLevel == WebBrowserPermissionLevel.Unrestricted;
		}

		public override SecurityElement ToXml ()
		{
			SecurityElement elem = new SecurityElement ("IPermission");
			elem.AddAttribute ("class", GetType ().AssemblyQualifiedName);
			elem.AddAttribute ("version", "1");
			if (IsUnrestricted ())
				elem.AddAttribute ("Unrestricted", bool.TrueString);
			else
				elem.AddAttribute ("Level", _webBrowserPermissionLevel.ToString ());
			return elem;
		}

		public override IPermission Union (IPermission target)
		{
			if (target == null)
				return Copy ();

			WebBrowserPermission other = target as WebBrowserPermission;
			if (other == null)
				throw new ArgumentException ("TargetNotWebBrowserPermissionLevel");

			WebBrowserPermissionLevel level = _webBrowserPermissionLevel > other._webBrowserPermissionLevel ? _webBrowserPermissionLevel : other._webBrowserPermissionLevel;
			return new WebBrowserPermission (level);
		}

		internal static void VerifyWebBrowserPermissionLevel (WebBrowserPermissionLevel level)
		{
			if (level < WebBrowserPermissionLevel.None || level > WebBrowserPermissionLevel.Unrestricted)
				throw new ArgumentException ("InvalidPermissionLevel");
		}
	}

}
