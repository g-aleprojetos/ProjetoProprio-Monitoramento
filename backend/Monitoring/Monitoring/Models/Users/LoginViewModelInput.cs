using System.ComponentModel.DataAnnotations;

namespace Monitoring.Models.Users
{
    public class LoginViewModelInput
    {
        [Required(ErrorMessage = "Login is required.")]
        public string Login { get; set; }

        [Required(ErrorMessage = "Password is required.")]
        public string Password { get; set; }
    }
}
