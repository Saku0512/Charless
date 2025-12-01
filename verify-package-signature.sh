#!/bin/bash
# ============================================
# Charless パッケージ署名確認スクリプト
# ============================================

set -e

# 色の定義
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 公開鍵のキーID
KEY_ID="B9B12A48239B3B49"
PUBLIC_KEY_FILE="gpg-public-key.asc"

echo "============================================"
echo "Charless パッケージ署名確認"
echo "============================================"
echo ""

# 引数チェック
if [ $# -eq 0 ]; then
    echo -e "${YELLOW}使用方法: $0 <package.deb|package.changes>${NC}"
    echo ""
    echo "例:"
    echo "  $0 charless_0.0.1_amd64.deb"
    echo "  $0 charless_0.0.1_amd64.changes"
    exit 1
fi

PACKAGE_FILE="$1"

# ファイルの存在確認
if [ ! -f "$PACKAGE_FILE" ]; then
    echo -e "${RED}エラー: ファイルが見つかりません: $PACKAGE_FILE${NC}"
    exit 1
fi

# 公開鍵のインポート確認
echo "1. 公開鍵の確認中..."
if ! gpg --list-keys "$KEY_ID" > /dev/null 2>&1; then
    echo -e "${YELLOW}警告: 公開鍵がインポートされていません${NC}"
    
    if [ -f "$PUBLIC_KEY_FILE" ]; then
        echo "公開鍵をインポートしますか? (y/n)"
        read -r response
        if [ "$response" = "y" ] || [ "$response" = "Y" ]; then
            gpg --import "$PUBLIC_KEY_FILE"
            echo -e "${GREEN}公開鍵をインポートしました${NC}"
        else
            echo -e "${RED}公開鍵をインポートしてください: gpg --import $PUBLIC_KEY_FILE${NC}"
            exit 1
        fi
    else
        echo -e "${RED}エラー: 公開鍵ファイルが見つかりません: $PUBLIC_KEY_FILE${NC}"
        exit 1
    fi
else
    echo -e "${GREEN}公開鍵が見つかりました${NC}"
fi

echo ""

# ファイルタイプの判定
if [[ "$PACKAGE_FILE" == *.changes ]]; then
    echo "2. .changesファイルの署名を確認中..."
    if command -v dpkg-sig > /dev/null 2>&1; then
        if dpkg-sig --verify "$PACKAGE_FILE"; then
            echo -e "${GREEN}✓ 署名確認成功: $PACKAGE_FILE${NC}"
        else
            echo -e "${RED}✗ 署名確認失敗: $PACKAGE_FILE${NC}"
            exit 1
        fi
    else
        echo "dpkg-sigが見つかりません。GPGで直接確認します..."
        if gpg --verify "$PACKAGE_FILE" 2>&1 | grep -q "Good signature"; then
            echo -e "${GREEN}✓ 署名確認成功: $PACKAGE_FILE${NC}"
        else
            echo -e "${RED}✗ 署名確認失敗: $PACKAGE_FILE${NC}"
            gpg --verify "$PACKAGE_FILE"
            exit 1
        fi
    fi
elif [[ "$PACKAGE_FILE" == *.deb ]]; then
    echo "2. .debファイルの署名を確認中..."
    if command -v dpkg-sig > /dev/null 2>&1; then
        if dpkg-sig --verify "$PACKAGE_FILE"; then
            echo -e "${GREEN}✓ 署名確認成功: $PACKAGE_FILE${NC}"
        else
            echo -e "${RED}✗ 署名確認失敗: $PACKAGE_FILE${NC}"
            exit 1
        fi
    elif command -v debsigs > /dev/null 2>&1; then
        if debsigs --verify "$PACKAGE_FILE"; then
            echo -e "${GREEN}✓ 署名確認成功: $PACKAGE_FILE${NC}"
        else
            echo -e "${RED}✗ 署名確認失敗: $PACKAGE_FILE${NC}"
            exit 1
        fi
    else
        echo -e "${YELLOW}警告: dpkg-sig または debsigs が見つかりません${NC}"
        echo "パッケージの内容を確認します..."
        dpkg-deb --info "$PACKAGE_FILE" | head -20
        echo ""
        echo -e "${YELLOW}注意: .debファイルの署名確認には dpkg-sig または debsigs が必要です${NC}"
        echo "インストール: sudo apt install dpkg-sig または sudo apt install debsigs"
    fi
else
    echo -e "${RED}エラー: サポートされていないファイル形式です${NC}"
    echo "対応形式: .deb, .changes"
    exit 1
fi

echo ""
echo "============================================"
echo -e "${GREEN}署名確認完了${NC}"
echo "============================================"

