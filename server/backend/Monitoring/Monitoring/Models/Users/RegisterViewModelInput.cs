using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace Monitoring.Models.Users
{
    public class RegisterViewModelInput
    {
        [Required(ErrorMessage = "Login is required.")]
        public string Login { get; set; }
       
        [Required(ErrorMessage = "E-mail is required.")]
        public string Email { get; set; }

        [Required(ErrorMessage = "Password is required.")]
        public string Password { get; set; }
    }
}
