/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/qt/gui/drawable_object_item.h>

#include <vclib/render/drawable/abstract_drawable_mesh.h>

namespace vcl::qt {

DrawableObjectItem::DrawableObjectItem(
    const std::shared_ptr<DrawableObject>& obj,
    QTreeWidget* parent) : QTreeWidgetItem(parent), mObj(obj)
{
    assert(obj);
    if (obj->isVisible())
        setCheckState(0, Qt::Checked);
    else
        setCheckState(0, Qt::Unchecked);

    // leave setText and setFlags AFTER setCheckState.
    // for some reason, they trigger the itemCheckStateChanged signal...
    setFlags(
        Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    setText(0, QString::fromStdString(obj->name()));

    // add mesh item
    addMeshItem();
}

std::shared_ptr<DrawableObject> DrawableObjectItem::drawableObject() const
{
    return mObj;
}

void DrawableObjectItem::addMeshItem()
{
    // clear any existing children
    auto l = takeChildren();
    for (auto* item : std::as_const(l)) {
        delete item;
    }

    // if the DrawbaleObject is an AbstractDrawableMesh
    auto mesh = std::dynamic_pointer_cast<AbstractDrawableMesh>(mObj);
    if (mesh) {
        addMeshInfoItem(*mesh);
        addTransformMatrixItem(*mesh);
        addMaterialsItem(*mesh);
    }
}

void DrawableObjectItem::addMeshInfoItem(const AbstractDrawableMesh& mesh)
{
    // make this item expandable, and add a child called "Mesh Info"
    auto meshInfoItem = new QTreeWidgetItem(this);
    meshInfoItem->setText(0, "Mesh Info");
    makeItemNotSelectable(meshInfoItem);

    // vertex number item
    auto vertexNumberItem = new QTreeWidgetItem(meshInfoItem);
    vertexNumberItem->setText(0, "# Vertices");
    vertexNumberItem->setText(1, QString::number(mesh.vertexNumber()));
    makeItemNotSelectable(vertexNumberItem);

    if (mesh.faceNumber() > 0) {
        auto faceNumberItem = new QTreeWidgetItem(meshInfoItem);
        faceNumberItem->setText(0, "# Faces");
        faceNumberItem->setText(1, QString::number(mesh.faceNumber()));
        makeItemNotSelectable(faceNumberItem);
    }

    if (mesh.edgeNumber() > 0) {
        auto edgeNumberItem = new QTreeWidgetItem(meshInfoItem);
        edgeNumberItem->setText(0, "# Edges");
        edgeNumberItem->setText(1, QString::number(mesh.edgeNumber()));
        makeItemNotSelectable(edgeNumberItem);
    }
}

void DrawableObjectItem::addTransformMatrixItem(
    const AbstractDrawableMesh& mesh)
{
    auto transformMatrixItem = new QTreeWidgetItem(this);
    transformMatrixItem->setText(0, "Transform Matrix");
    makeItemNotSelectable(transformMatrixItem);

    // add 4 rows for the 4x4 matrix
    for (int i = 0; i < 4; ++i) {
        auto rowItem = new QTreeWidgetItem(transformMatrixItem);

        // use a monospace font for the matrix
        rowItem->setFont(1, QFont("Courier New", 10));

        rowItem->setText(0, "");
        QString rowLabel = "";
        for (int j = 0; j < 4; ++j) {
            rowLabel += QString::number(mesh.transformMatrix()(i, j), 'f', 3);
            if (j < 3)
                rowLabel += "\t";
        }
        rowItem->setText(1, rowLabel);
        makeItemNotSelectable(rowItem);
    }
}

void DrawableObjectItem::addMaterialsItem(const AbstractDrawableMesh& mesh)
{
    if (mesh.materials().size() > 0) {
        auto materialsItem = new QTreeWidgetItem(this);
        materialsItem->setText(0, "Materials");
        makeItemNotSelectable(materialsItem);

        uint i = 0;
        for (const auto& material : mesh.materials()) {
            auto materialItem = new QTreeWidgetItem(materialsItem);
            materialItem->setText(0, QString::number(i));
            std::string name = material.name();
            if (name.empty()) {
                name = "material_" + std::to_string(i);
            }
            materialItem->setText(1, QString::fromStdString(name));
            makeItemNotSelectable(materialItem);

            addMaterialData(mesh, material, materialItem);

            i++;
        }
    }
}

void DrawableObjectItem::addMaterialData(
    const AbstractDrawableMesh& mesh,
    const Material&  material,
    QTreeWidgetItem* parent)
{
    using enum Material::TextureType;

    // base color
    auto baseColorItem = new QTreeWidgetItem(parent);
    baseColorItem->setText(0, "Base Color");
    baseColorItem->setText(
        1,
        QString::fromStdString(
            "(" + std::to_string(material.baseColor().red()) + ", " +
            std::to_string(material.baseColor().green()) + ", " +
            std::to_string(material.baseColor().blue()) + ", " +
            std::to_string(material.baseColor().alpha()) + ")"));
    makeItemNotSelectable(baseColorItem);

    // metallic
    auto metallicItem = new QTreeWidgetItem(parent);
    metallicItem->setText(0, "Metallic");
    metallicItem->setText(
        1, QString::number(material.metallic(), 'f', 3));
    makeItemNotSelectable(metallicItem);

    // roughness
    auto roughnessItem = new QTreeWidgetItem(parent);
    roughnessItem->setText(0, "Roughness");
    roughnessItem->setText(
        1, QString::number(material.roughness(), 'f', 3));
    makeItemNotSelectable(roughnessItem);

    // emissive color
    if (material.emissiveColor() != Color::Black) {
        auto emissiveColorItem = new QTreeWidgetItem(parent);
        emissiveColorItem->setText(0, "Emissive Color");
        emissiveColorItem->setText(
            1,
            QString::fromStdString(
                "(" + std::to_string(material.emissiveColor().red()) + ", " +
                std::to_string(material.emissiveColor().green()) + ", " +
                std::to_string(material.emissiveColor().blue()) + ")"));
        makeItemNotSelectable(emissiveColorItem);
    }

    // alpha mode
    if (material.alphaMode() != Material::AlphaMode::ALPHA_OPAQUE) {
        auto alphaModeItem = new QTreeWidgetItem(parent);
        alphaModeItem->setText(0, "Alpha Mode");
        QString alphaModeStr;
        switch (material.alphaMode()) {
            case Material::AlphaMode::ALPHA_OPAQUE:
                alphaModeStr = "OPAQUE";
                break;
            case Material::AlphaMode::ALPHA_MASK:
                alphaModeStr = "MASK";
                break;
            case Material::AlphaMode::ALPHA_BLEND:
                alphaModeStr = "BLEND";
                break;
        }
        alphaModeItem->setText(1, alphaModeStr);
        makeItemNotSelectable(alphaModeItem);
    }

    // alpha cutoff
    if (material.alphaMode() == Material::AlphaMode::ALPHA_MASK) {
        auto alphaCutoffItem = new QTreeWidgetItem(parent);
        alphaCutoffItem->setText(0, "Alpha Cutoff");
        alphaCutoffItem->setText(
            1, QString::number(material.alphaCutoff(), 'f', 3));
        makeItemNotSelectable(alphaCutoffItem);
    }

    auto addTxtItem = [&](const Texture& txt, const QString& txtName) {
        if (!txt.isNull()) {
            auto textureItem = new QTreeWidgetItem(parent);
            textureItem->setText(0, txtName);

            QString text = QString::fromStdString(txt.path());

            const auto& image = mesh.textureImage(txt.path());

            text += " (";
            text += image.isNull() ? "not loaded" :
                                     QString::number(image.width()) + " x " +
                                         QString::number(image.height());
            text += ")";

            textureItem->setText(1, text);
            makeItemNotSelectable(textureItem);
        }
    };

    // base color Texture
    const Texture& bcTxt = material.texture(BASE_COLOR);
    addTxtItem(bcTxt, "Base Color Texture");

    // metalling roughness Texture
    const Texture& mrTxt = material.texture(METALLIC_ROUGHNESS);
    addTxtItem(mrTxt, "Metallic Roughness Texture");

    // normal Texture
    const Texture& nTxt = material.texture(NORMAL);
    addTxtItem(nTxt, "Normal Texture");

    // occlusion Texture
    const Texture& oTxt = material.texture(OCCLUSION);
    addTxtItem(oTxt, "Occlusion Texture");

    // emissive Texture
    const Texture& eTxt = material.texture(EMISSIVE);
    addTxtItem(eTxt, "Emissive Texture");

    // double sided
    if (material.doubleSided()) {
        auto doubleSidedItem = new QTreeWidgetItem(parent);
        doubleSidedItem->setText(0, "Double Sided");
        doubleSidedItem->setText(1, "True");
        makeItemNotSelectable(doubleSidedItem);
    }
}

void DrawableObjectItem::makeItemNotSelectable(QTreeWidgetItem* item)
{
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
}

} // namespace vcl::qt
