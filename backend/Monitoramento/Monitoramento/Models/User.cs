using System.ComponentModel.DataAnnotations.Schema;

namespace Monitoramento.Models
{
    [Table("Users")]
    public class User
    {
        public int UserId { get; set; }
        public string Name { get; set; }
        public string Password { get; set; }
        public string Email { get; set; }

    }
}
