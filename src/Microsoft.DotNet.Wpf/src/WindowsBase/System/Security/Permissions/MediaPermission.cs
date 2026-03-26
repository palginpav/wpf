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

	public sealed class MediaPermission : CodeAccessPermission, IUnrestrictedPermission
	{
		private MediaPermissionAudio _mediaPermissionAudio;
		private MediaPermissionVideo _mediaPermissionVideo;
		private MediaPermissionImage _mediaPermissionImage;

		public MediaPermission ()
		{
			InitDefaults ();
		}

		public MediaPermission (MediaPermissionAudio permissionAudio)
		{
			VerifyMediaPermissionAudio (permissionAudio);
			InitDefaults ();
			_mediaPermissionAudio = permissionAudio;
		}

		public MediaPermission (MediaPermissionImage permissionImage)
		{
			VerifyMediaPermissionImage (permissionImage);
			InitDefaults ();
			_mediaPermissionImage = permissionImage;
		}

		public MediaPermission (MediaPermissionVideo permissionVideo)
		{
			VerifyMediaPermissionVideo (permissionVideo);
			InitDefaults ();
			_mediaPermissionVideo = permissionVideo;
		}

		public MediaPermission (PermissionState state)
		{
			if (state == PermissionState.Unrestricted) {
				_mediaPermissionAudio = MediaPermissionAudio.AllAudio;
				_mediaPermissionVideo = MediaPermissionVideo.AllVideo;
				_mediaPermissionImage = MediaPermissionImage.AllImage;
			} else if (state == PermissionState.None) {
				_mediaPermissionAudio = MediaPermissionAudio.NoAudio;
				_mediaPermissionVideo = MediaPermissionVideo.NoVideo;
				_mediaPermissionImage = MediaPermissionImage.NoImage;
			} else {
				throw new ArgumentException ("InvalidPermissionState");
			}
		}

		public MediaPermission (MediaPermissionAudio permissionAudio, MediaPermissionVideo permissionVideo, MediaPermissionImage permissionImage)
		{
			VerifyMediaPermissionAudio (permissionAudio);
			VerifyMediaPermissionVideo (permissionVideo);
			VerifyMediaPermissionImage (permissionImage);
			_mediaPermissionAudio = permissionAudio;
			_mediaPermissionVideo = permissionVideo;
			_mediaPermissionImage = permissionImage;
		}

		public MediaPermissionAudio Audio {
			get { return _mediaPermissionAudio; }
		}

		public MediaPermissionVideo Video {
			get { return _mediaPermissionVideo; }
		}

		public MediaPermissionImage Image {
			get { return _mediaPermissionImage; }
		}

		public override IPermission Copy ()
		{
			return new MediaPermission (_mediaPermissionAudio, _mediaPermissionVideo, _mediaPermissionImage);
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
				_mediaPermissionAudio = MediaPermissionAudio.AllAudio;
				_mediaPermissionVideo = MediaPermissionVideo.AllVideo;
				_mediaPermissionImage = MediaPermissionImage.AllImage;
				return;
			}

			InitDefaults ();

			string audio = securityElement.Attribute ("Audio");
			if (audio != null)
				_mediaPermissionAudio = (MediaPermissionAudio) Enum.Parse (typeof (MediaPermissionAudio), audio);
			else
				throw new ArgumentException ("BadXml: audio");

			string video = securityElement.Attribute ("Video");
			if (video != null)
				_mediaPermissionVideo = (MediaPermissionVideo) Enum.Parse (typeof (MediaPermissionVideo), video);
			else
				throw new ArgumentException ("BadXml: video");

			string image = securityElement.Attribute ("Image");
			if (image != null)
				_mediaPermissionImage = (MediaPermissionImage) Enum.Parse (typeof (MediaPermissionImage), image);
			else
				throw new ArgumentException ("BadXml: image");
		}

		public override IPermission Intersect (IPermission target)
		{
			if (target == null)
				return null;

			MediaPermission other = target as MediaPermission;
			if (other == null)
				throw new ArgumentException ("TargetNotMediaPermissionLevel");

			MediaPermissionAudio audio = _mediaPermissionAudio < other._mediaPermissionAudio ? _mediaPermissionAudio : other._mediaPermissionAudio;
			MediaPermissionVideo video = _mediaPermissionVideo < other._mediaPermissionVideo ? _mediaPermissionVideo : other._mediaPermissionVideo;
			MediaPermissionImage image = _mediaPermissionImage < other._mediaPermissionImage ? _mediaPermissionImage : other._mediaPermissionImage;

			return new MediaPermission (audio, video, image);
		}

		public override bool IsSubsetOf (IPermission target)
		{
			if (target == null)
				return EqualsLevel (MediaPermissionAudio.NoAudio, MediaPermissionVideo.NoVideo, MediaPermissionImage.NoImage);

			MediaPermission other = target as MediaPermission;
			if (other == null)
				throw new ArgumentException ("TargetNotMediaPermissionLevel");

			return _mediaPermissionAudio <= other._mediaPermissionAudio &&
			       _mediaPermissionVideo <= other._mediaPermissionVideo &&
			       _mediaPermissionImage <= other._mediaPermissionImage;
		}

		public bool IsUnrestricted ()
		{
			return EqualsLevel (MediaPermissionAudio.AllAudio, MediaPermissionVideo.AllVideo, MediaPermissionImage.AllImage);
		}

		public override SecurityElement ToXml ()
		{
			SecurityElement elem = new SecurityElement ("IPermission");
			elem.AddAttribute ("class", GetType ().AssemblyQualifiedName);
			elem.AddAttribute ("version", "1");
			if (IsUnrestricted ()) {
				elem.AddAttribute ("Unrestricted", bool.TrueString);
			} else {
				elem.AddAttribute ("Audio", _mediaPermissionAudio.ToString ());
				elem.AddAttribute ("Video", _mediaPermissionVideo.ToString ());
				elem.AddAttribute ("Image", _mediaPermissionImage.ToString ());
			}
			return elem;
		}

		public override IPermission Union (IPermission target)
		{
			if (target == null)
				return Copy ();

			MediaPermission other = target as MediaPermission;
			if (other == null)
				throw new ArgumentException ("TargetNotMediaPermissionLevel");

			MediaPermissionAudio audio = _mediaPermissionAudio > other._mediaPermissionAudio ? _mediaPermissionAudio : other._mediaPermissionAudio;
			MediaPermissionVideo video = _mediaPermissionVideo > other._mediaPermissionVideo ? _mediaPermissionVideo : other._mediaPermissionVideo;
			MediaPermissionImage image = _mediaPermissionImage > other._mediaPermissionImage ? _mediaPermissionImage : other._mediaPermissionImage;

			return new MediaPermission (audio, video, image);
		}

		internal static void VerifyMediaPermissionAudio (MediaPermissionAudio level)
		{
			if (level < MediaPermissionAudio.NoAudio || level > MediaPermissionAudio.AllAudio)
				throw new ArgumentException ("InvalidPermissionLevel");
		}

		internal static void VerifyMediaPermissionVideo (MediaPermissionVideo level)
		{
			if (level < MediaPermissionVideo.NoVideo || level > MediaPermissionVideo.AllVideo)
				throw new ArgumentException ("InvalidPermissionLevel");
		}

		internal static void VerifyMediaPermissionImage (MediaPermissionImage level)
		{
			if (level < MediaPermissionImage.NoImage || level > MediaPermissionImage.AllImage)
				throw new ArgumentException ("InvalidPermissionLevel");
		}

		private void InitDefaults ()
		{
			_mediaPermissionAudio = MediaPermissionAudio.SafeAudio;
			_mediaPermissionVideo = MediaPermissionVideo.SafeVideo;
			_mediaPermissionImage = MediaPermissionImage.SafeImage;
		}

		private bool EqualsLevel (MediaPermissionAudio audioLevel, MediaPermissionVideo videoLevel, MediaPermissionImage imageLevel)
		{
			return _mediaPermissionAudio == audioLevel &&
			       _mediaPermissionVideo == videoLevel &&
			       _mediaPermissionImage == imageLevel;
		}
	}

}
