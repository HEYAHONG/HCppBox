# 说明

 简易的HTTP/1.1(HTTP/1.0) 服务端，一般用于静态文件服务。

**注意：绝大部分系统会占用80端口（默认HTTP端口），故而无法使用默认参数绑定地址，需要通过命令选项指定一个未使用的端口**

# 环境变量

- | 名称                   | 说明         | 备注                                |
  | ---------------------- | ------------ | ----------------------------------- |
  | `HTTPD_PORT`           | 端口         | 默认为80                            |
  | `HTTPD_IDLE_TIMEOUT`   | 空闲超时     | 默认为5                             |
  | `HTTPD_INDEX_FILENAME` | 引索文件名称 | 默认为`index.html`                  |
  | `HTTPD_ROOT_PATH`      | 服务器根目录 | 默认为当前工作目录                  |
  | `HTTPD_SSL_CERT_PATH`  | SSL证书路径  | 若需要SSL，证书与密钥必须同时具备。 |
  | `HTTPD_SSL_KEY_PATH`   | SSL密钥路径  | 若需要SSL，证书与密钥必须同时具备。 |
  

# 证书

对于启用TLS/SSL的服务而言，需要PEM格式的证书及私钥，其中私钥不可密码加密。

对于具有域名的用户，可使用`certbot`（对于最新ubuntu系统，可直接安装`certbot`软件包）签发免费证书（[https://letsencrypt.org/](https://letsencrypt.org/)），不过需要提前配置http服务（非TLS/SSL）并且使公网可访问`/.well-known/acme-challenge/` 路径。

对于测试用户，可自行使用各种工具签发测试证书，本人推荐使用[https://github.com/OpenVPN/easy-rsa.git](https://github.com/OpenVPN/easy-rsa.git)（对于最新ubuntu系统，可直接安装`easy-rsa`软件包），注意需要签发没有密码加密的私钥且测试时应该忽略浏览器的安全警告。

## 测试证书

测试证书由easy-rsa管理，其目录在[../../common/pki](../../common/pki)，若需要浏览器不警告，可在浏览器的设置中添加[测试ca证书](../../common/pki/ca.crt)（测试完需要删除，否则有一定风险）。

- localhost:[cert](../../common/pki/issued/localhost.crt)、[key](../../common/pki/private/localhost.key)。

# MIME

对于服务器而言，若需要浏览器正确处理相应文件，而不是触发下载文件，HTTP服务器应该在头部字段`Content-Type`中放入正确的MIME类型。

本工程内置了静态服务器常见的格式及后缀，具体见[httpd.cpp](httpd.cpp),对于不支持的文件格式，默认返回`application/octet-stream`，一般会触发下载行为。

更多MIME类型定义可见常用的MIME数据库[mime-db](https://github.com/jshttp/mime-db)。

