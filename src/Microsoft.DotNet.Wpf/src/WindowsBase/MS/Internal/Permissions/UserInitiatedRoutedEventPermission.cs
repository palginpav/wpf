using System;
using System.Security.Permissions;

namespace MS.Internal.Permissions
{
    [Serializable]
    internal sealed class UserInitiatedRoutedEventPermission : InternalParameterlessPermissionBase
    {
        public UserInitiatedRoutedEventPermission() : base(PermissionState.Unrestricted) { }
        public UserInitiatedRoutedEventPermission(PermissionState state) : base(state) { }
    }
}
