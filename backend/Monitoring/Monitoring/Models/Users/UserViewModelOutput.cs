using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Monitoring.Models.Users
{
    public class UserViewModelOutput
    {
        public int Code { get; set; }
        public string Login { get; set; }
        public string Email { get; set; }
    }
}
