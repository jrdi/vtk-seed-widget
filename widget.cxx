#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCommand.h>
#include <vtkDebugLeaks.h>
#include <vtkCamera.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkMath.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetEventTranslator.h>
#include <vtkImageViewer2.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>
#include <vtkImageActorPointPlacer.h>

int main( int argc, char *argv[] )
{
  if( argc != 3 )
    {
    std::cerr << "Usage:" << std::endl;
    std::cerr << argv[0] << " <Image (*.jpg)> <output filename (*.txt)> (<-Scale|-Shift>)" << std::endl;

    return EXIT_FAILURE;
    }

  vtkSmartPointer<vtkJPEGReader> jpegReader = vtkSmartPointer<vtkJPEGReader>::New();
  jpegReader->SetFileName( argv[1] );
  jpegReader->Update();

  vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
  imageViewer->SetInputConnection( jpegReader->GetOutputPort() );

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  imageViewer->SetupInteractor(renderWindowInteractor);

  vtkSmartPointer<vtkRenderer> renderer = imageViewer->GetRenderer();
  double imageBounds[6];
  imageViewer->GetImageActor()->GetBounds(imageBounds);

  vtkSmartPointer<vtkImageActorPointPlacer> placer = vtkSmartPointer<vtkImageActorPointPlacer>::New();
  placer->SetImageActor( imageViewer->GetImageActor() );

  vtkSmartPointer<vtkPointHandleRepresentation2D> handle = 
    vtkSmartPointer<vtkPointHandleRepresentation2D>::New();
  handle->GetProperty()->SetColor(1,0,0);
  handle->SetPointPlacer( placer );

  vtkSmartPointer<vtkSeedRepresentation> seedRep = 
    vtkSmartPointer<vtkSeedRepresentation>::New();
  seedRep->SetHandleRepresentation(handle);
 
  vtkSmartPointer<vtkSeedWidget> seedWidget = 
      vtkSmartPointer<vtkSeedWidget>::New();
  seedWidget->SetInteractor(renderWindowInteractor);
  seedWidget->SetRepresentation(seedRep);
  seedWidget->On();
  
  seedWidget->Render();
  renderer->ResetCamera();
 
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();
 
  seedWidget->Off();

  int N = seedRep->GetNumberOfSeeds();

  std::ofstream ofs;
  ofs.open( argv[2] );

  if( !ofs.is_open() )
    {
    std::cerr << "can't open " << argv[2] << std::endl;
    return EXIT_FAILURE;
    }

  for( int i = 0; i < N; i++ )
    {
      double pos[3];
      seedRep->GetSeedDisplayPosition( i, pos );
      ofs << pos[0] << " " << imageBounds[3] - pos[1] << std::endl;
    }
  ofs.close();
 
  return EXIT_SUCCESS;
}