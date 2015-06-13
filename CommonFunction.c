void Delay(unsigned int i)      //延时程序,i是形式参数,i为1时 延时约1MS
{
unsigned int j;
for(;i>0;i--)             //变量i由实际参数传入一个值,因此i不能赋初值
for(j=0;j<125;j++) //j由0自增到125 所用的时间大约是1MS       
{;}
}
