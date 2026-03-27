using System;
using System.Security.Permissions;

namespace MS.Internal.Permissions
{
    [Serializable]
    internal sealed class CompoundFileIOPermission : InternalParameterlessPermissionBase
    {
        public CompoundFileIOPermission() : base(PermissionState.Unrestricted) { }
        public CompoundFileIOPermission(PermissionState state) : base(state) { }
    }
}
