namespace Monitoring.Entities
{
    public class DeviceEntity
    {
        public int Code { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public virtual UserEntity UserEntity { get; set; }
    }
}
