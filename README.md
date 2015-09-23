# move-recommendation

move recommendation system 

利用 SVD 分解 协同过滤，实现简单的电影推荐。首先根据training.txt文件进行模型训练，生成模型并存储到svd_model_.txt文件中
之后，加载模型并读取测试文件test.txt生成预测的数据并输出到文件中，之后把这个输出文件上传到一个自动测试网站，根据RMSE衡量你所预测的数据的准确度。

void SVD(string configureFile, string trainDataFile, string modelSaveFile) 模型训练


void  Predict(string configureFile, string modelSaveFile, string testDataFile, string resultSaveFile) 预测结果

配置文件svd.conf包含了在模型训练中用到的一些参数设置，这是最重要的配置文件。