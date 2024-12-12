# Smart Factory Quality Monitoring System

## 開發背景
## 產品特點

---

# System architecture
![插入圖片]()

---

# Hardware – Arduino

---

# Software – AWS Cloud

## 本機端

1. 程式檔案：
   - 本機寫 `SendData.py` 程式，用於傳送數據到 AWS Cloud 的 IoT SiteWise 。
2. 安裝必要套件：
   - 確保已安裝 AWS SDK for Python (boto3) 及其他所需的 Python 套件，例如 pandas。 
   - 安裝指令：`pip install boto3`
3. 設置 AWS IAM 憑證：
   - 進入 AWS IAM 建立 User ，同時建立新的 Access Key。
   - 在 `SendData.py` 程式中，填入 AWS Cloud 提供的 Access_key 和 Secret_key，利用這些憑證建立與 AWS 的安全連線。
4. 程式傳送數據：
   - 在 `SendData.py` 程式中，設定需要傳送到 IoT SiteWise 的數據格式，例如測量值（如重量）。程式會以指定的時間間隔將數據傳送到 AWS。

---

# AWS Cloud

## AWS IoT SiteWise

1. IAM 使用者設置：
   - 登入 AWS Management Console，進入 IAM 服務。
   - 建立新的 IAM 使用者，分配對 IoT SiteWise 的權限（`AWSIoTSiteWiseFullAccess`）。
   - 為該使用者生成 Access Key 和 Secret Key，這些憑證將用於本機程式的連線配置。
2. IoT SiteWise 建立資產模型與資產：
   - 進入 IoT SiteWise 服務，點擊 Models，建立一個新的模型：
     - 命名為 `WeightModel`。
     - 在模型中加入以下兩類定義：
       - Attribute Definitions：用於記錄靜態屬性（例如設備名稱或規格）。
       - Measurement Definitions：用於接收測量數據（例如重量數據）。
     - 儲存模型。
   - 基於剛才建立的 `WeightModel`，新增資產（Asset）：
     - 資產名稱：`Weight1`。
     - 在新增過程中，指定 Measurement 的 alias，例如 `/factory/Weight_test/Weight`，用於數據的傳送和檢索。
   - 完成後，確認資產頁面，可以利用本機執行 `SendData.py`，確認傳送資料是否顯示在 Latest value（最新數據值）以及 Latest value timestamp（最新數據的時間戳）。

---

## Amazon Managed Grafana

1. 建立 Grafana Workspace：
   - 進入 Amazon Managed Grafana，點擊 Create workspace。
   - 在建立過程中：
     - 命名為 `foodfactoryDemo`。
     - Permission type：選擇 `Service managed`。
     - 選擇 AWS IAM Identity Center 作為身份管理方法。
     - 在資料來源（Data Source）中選擇 AWS IoT SiteWise。
   - 建立完成後，將需要使用 Grafana 的使用者新增到 Workspace 的使用者清單中。
   - 點擊生成的 Grafana URL，登入 Grafana Workspace。
2. 建立儀表板：
   - 在 Grafana Workspace 中，新增資料來源，選擇 AWS IoT SiteWise。
   - 建立新的儀表板，選擇 IoT SiteWise 作為數據來源。
   - 選擇之前在 SiteWise 中建立的資產 `Weight1`，並選擇其 Measurement 數據作為 Time Series 資料顯示在儀表板上。
   - 配置儀表板樣式，例如：折線圖。

---

## 流程完成與確認

1. 執行本機的 `SendData.py` 程式，將測量數據傳送到 AWS Cloud。
2. 登入 AWS SiteWise，確認資產 `Weight1` 中的 Measurements 欄位能顯示最新數據及時間戳。
3. 登入 Grafana，確認儀表板能即時顯示傳送的數據。
