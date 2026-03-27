using System;
using System.Security;
using System.Security.Permissions;

namespace MS.Internal.Permissions
{
    [Serializable]
    internal abstract class InternalParameterlessPermissionBase : CodeAccessPermission, IUnrestrictedPermission
    {
        private bool _unrestricted;

        protected InternalParameterlessPermissionBase(PermissionState state)
        {
            if (state == PermissionState.Unrestricted)
                _unrestricted = true;
            else if (state == PermissionState.None)
                _unrestricted = false;
            else
                throw new ArgumentException("InvalidPermissionStateValue", "state");
        }

        public bool IsUnrestricted() { return _unrestricted; }

        public override IPermission Copy()
        {
            return (InternalParameterlessPermissionBase)MemberwiseClone();
        }

        public override IPermission Intersect(IPermission target)
        {
            if (target == null) return null;
            var other = target as InternalParameterlessPermissionBase;
            if (other == null) throw new ArgumentException("target");
            return (_unrestricted && other._unrestricted) ? Copy() : null;
        }

        public override bool IsSubsetOf(IPermission target)
        {
            if (target == null) return !_unrestricted;
            var other = target as InternalParameterlessPermissionBase;
            if (other == null) throw new ArgumentException("target");
            return !_unrestricted || other._unrestricted;
        }

        public override IPermission Union(IPermission target)
        {
            if (target == null) return Copy();
            var other = target as InternalParameterlessPermissionBase;
            if (other == null) throw new ArgumentException("target");
            var p = (InternalParameterlessPermissionBase)Copy();
            if (other._unrestricted) p._unrestricted = true;
            return p;
        }

        public override SecurityElement ToXml()
        {
            var e = new SecurityElement("IPermission");
            e.AddAttribute("class", GetType().AssemblyQualifiedName);
            e.AddAttribute("version", "1");
            if (_unrestricted) e.AddAttribute("Unrestricted", "true");
            return e;
        }

        public override void FromXml(SecurityElement elem)
        {
            if (elem == null) throw new ArgumentNullException("elem");
            string u = elem.Attribute("Unrestricted");
            _unrestricted = u != null && string.Equals(u, "true", StringComparison.OrdinalIgnoreCase);
        }
    }
}
