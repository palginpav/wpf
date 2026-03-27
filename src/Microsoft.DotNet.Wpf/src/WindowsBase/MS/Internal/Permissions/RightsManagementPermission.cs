using System;
using System.Security.Permissions;

namespace MS.Internal.Permissions
{
    [Serializable]
    internal sealed class RightsManagementPermission : InternalParameterlessPermissionBase
    {
        public RightsManagementPermission() : base(PermissionState.Unrestricted) { }
        public RightsManagementPermission(PermissionState state) : base(state) { }
    }
}
